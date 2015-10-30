// Copyright (C) 2012-2015 Leap Motion, Inc. All rights reserved.
#pragma once
#include "auto_signal.h"

namespace autowiring {

/// <summary>
/// An unsynchronzied wrapper type that implements the observable pattern
/// </summary>
/// <remarks>
/// 
/// </remarks>
template<typename T>
class observable {
public:
  observable(void) = default;

  observable(const T& val) :
    val(val)
  {}

  observable(T&& val) :
    val(std::move(val))
  {}

  observable(observable&& rhs) :
    val(std::move(rhs.val)),
    onChanged(std::move(rhs.onChanged)),
    onBeforeChanged(std::move(rhs.onBeforeChanged))
  {}

  autowiring::signal<void()> onChanged;
  autowiring::signal<void(const T& oldValue, const T& newValue)> onBeforeChanged;

private:
  T val;

public:
  operator const T&(void) const { return val; }
  const T& operator*(void) const { return val; }

  observable& operator=(const T& rhs) {
    onBeforeChanged(val, rhs);
    val = rhs;
    onChanged();
    return *this;
  }
};

}