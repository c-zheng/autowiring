// Copyright (C) 2012-2015 Leap Motion, Inc. All rights reserved.
#pragma once

class ThrowingListener {
public:
  ThrowingListener(void) {}
  virtual void DoThrow(void) = 0;
};
