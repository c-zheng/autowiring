// Copyright (c) 2010 - 2013 Leap Motion. All rights reserved. Proprietary and confidential.
#include "stdafx.h"
#include "ExceptionFilterTest.h"
#include "EventReceiver.h"
#include "ExceptionFilter.h"
#include <stdexcept>
#include <sstream>
#include <iostream>

using namespace std;

string FormatMessage(int value) {
  std::stringstream sstr;
  sstr << "custom_exception: " << value;
  return sstr.str();
}

class custom_exception:
  public std::exception
{
public:
  custom_exception(int value):
    m_value(value)
  {
  }

  int m_value;
};

/// <summary>
/// Exception type which can track its own destruction
/// </summary>
class tracking_exception:
  public std::exception
{
public:
  tracking_exception(int) {
    s_count++;
  }
  tracking_exception(const tracking_exception& rhs) {
    s_count++;
  }
  ~tracking_exception(void) {
    s_count--;
  }

  static size_t s_count;
};

size_t tracking_exception::s_count = 0;

class ThrowingListener:
  public virtual EventReceiver
{
public:
  virtual void DoThrow(void) = 0;
};

template<class Ex>
class ThrowsWhenRun:
  public CoreThread
{
public:
  ThrowsWhenRun(void) {
    Ready();
  }

  // This convoluted syntax is required to evade warnings on Mac
  decltype(throw_rethrowable Ex(100)) MakeException() {
    return throw_rethrowable Ex(100);
  }

  void Run(void) override {
    MakeException();
  }
};

class ThrowsWhenFired:
  public ThrowingListener
{
public:
  void DoThrow(void) override {
    throw_rethrowable custom_exception(200);
  }
};

class GenericFilter:
  public ExceptionFilter
{
public:
  GenericFilter(void):
    m_hit(false),
    m_specific(false),
    m_generic(false),
    m_fireSpecific(false)
  {
  }

  bool m_hit;
  bool m_specific;
  bool m_generic;
  bool m_fireSpecific;

  virtual void Filter(const std::function<void()>& rethrower) override {
    m_hit = true;
    try {
      rethrower();
    } catch(tracking_exception&) {
    } catch(custom_exception& custom) {
      EXPECT_EQ(100, custom.m_value) << "A filtered custom exception did not have the expected member field value";
      m_specific = true;
    } catch(...) {
      m_generic = true;
    }
  }

  virtual void Filter(const std::function<void()>& rethrower, const EventReceiverProxyBase* pProxy, EventReceiver* pRecipient) override {
    m_hit = true;
    try {
      rethrower();
    } catch(custom_exception& custom) {
      EXPECT_EQ(200, custom.m_value) << "A fired exception did not have the expected value, probable copy malfunction";
      m_fireSpecific = true;
    } catch(...) {
      m_generic = true;
    }
  }
};

TEST_F(ExceptionFilterTest, ExceptionDestruction) {
  // Add the exception filter type to the context first
  AutoRequired<GenericFilter> filter;

  // Now add something that will throw when it's run:
  AutoRequired<ThrowsWhenRun<tracking_exception>> thrower;

  // Run:
  m_create->InitiateCoreThreads();
  thrower->Wait();

  // Verify that the exception was destroyed the correct number of times:
  EXPECT_EQ(0, tracking_exception::s_count) << "Exception was not destroyed the correct number of times";
}

TEST_F(ExceptionFilterTest, CheckThrowThrow) {
  class example {
  public:
    example() {
      throw std::exception();
    }
  };

  EXPECT_THROW(throw example(), std::exception) << "An exception type which throws from its ctor did not throw the expected type";
}

TEST_F(ExceptionFilterTest, ThreadThrowsCheck) {
  // Add the exception filter type to the context first
  AutoRequired<GenericFilter> filter;

  // Now add something that will throw when it's run:
  AutoRequired<ThrowsWhenRun<custom_exception>> thrower;

  // Wait for the thrower to terminate, should be pretty fast:
  m_create->InitiateCoreThreads();
  thrower->Wait();

  // Hopefully the filter got hit in the right spot:
  EXPECT_TRUE(filter->m_hit) << "Filter was not invoked for a thrown exception";
  EXPECT_TRUE(filter->m_specific) << "Filter did not correctly detect the exception type";
  EXPECT_FALSE(filter->m_generic) << "Filter did not correctly filter out a specific exception";
}

TEST_F(ExceptionFilterTest, FireThrowsCheck) {
  // Add the generic filter:
  AutoRequired<GenericFilter> filter;

  // Add a context member which will throw when its event is fired, and a firing class:
  AutoRequired<ThrowsWhenFired> fireThrower;

  // Add something to fire the exception:
  AutoFired<ThrowingListener> broadcaster;
  broadcaster(&ThrowingListener::DoThrow)();

  // Verify that the exception was filtered properly by the generic filter:
  EXPECT_TRUE(filter->m_fireSpecific) << "Filter was not invoked on a Fired exception";
}

TEST_F(ExceptionFilterTest, EnclosedThrowCheck) {
  // Create our listener:
  AutoRequired<GenericFilter> filter;

  // Now the subcontext:
  AutoCreateContext subCtxt;
  CurrentContextPusher pshr(subCtxt);

  // Create and start:
  AutoRequired<ThrowsWhenRun<custom_exception>> runThrower;
  subCtxt->InitiateCoreThreads();

  // Wait for the exception to get thrown:
  subCtxt->Wait();

  // Verify that the filter caught the exception:
  EXPECT_TRUE(filter->m_hit) << "Filter operating in a superior context did not catch an exception thrown from a child context";
}