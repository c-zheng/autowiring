#include "stdafx.h"
#include "AutoJob.h"
#include "CoreContext.h"
#include <future>


AutoJob::AutoJob(const char* name) :
  ContextMember(name),
  m_canAccept(false)
{}

void AutoJob::FireEvent(DispatchThunkBase& thunk){
  std::async(std::launch::async, [&thunk]{
    thunk();
    delete &thunk;
  });
}

bool AutoJob::CanAccept(void) const {
  return m_canAccept;
}

bool AutoJob::DelayUntilCanAccept(void) {
  boost::unique_lock<boost::mutex> lk(m_jobLock);
  m_jobUpdate.wait(lk, [this]{
    return m_canAccept;
  });
  return m_canAccept;
}

bool AutoJob::IsRunning(void) const { return CanAccept(); }

bool AutoJob::Start(std::shared_ptr<Object> outstanding) {
  std::shared_ptr<CoreContext> context = m_context.lock();
  if(!context)
    return false;
  
  m_outstanding = outstanding;
  m_canAccept = true;
  
  m_jobUpdate.notify_all();
  
  return true;
}

void AutoJob::Stop(bool graceful) {
  m_canAccept = false;
  
  if (graceful){
    // Pend a call which will invoke Abort once the dispatch queue is done:
    DispatchQueue::Pend([this] {
      this->Abort();
    });
  } else {
    // Abort the dispatch queue so anyone waiting will wake up
    DispatchQueue::Abort();
  }
  
  try {
    // If we are asked to rundown while we still have elements in our dispatch queue,
    // we must try to process them:
    DispatchAllEvents();
  }
  catch(...) {
    // We failed to run down the dispatch queue gracefully, we now need to abort it
    DispatchQueue::Abort();
  }
  
  m_outstanding.reset();
}