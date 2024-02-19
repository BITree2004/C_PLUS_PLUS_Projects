#pragma once

#include "test-object.h"

template <typename T>
struct tracking_deleter {
  explicit tracking_deleter(bool* deleted) : deleted(deleted) {}

  void operator()(T* object) {
    *deleted = true;
    delete object;
  }

private:
  bool* deleted;
};

struct destruction_tracker_base {
  destruction_tracker_base() = default;

  destruction_tracker_base(const destruction_tracker_base&) = delete;
  destruction_tracker_base& operator=(const destruction_tracker_base&) = delete;

  destruction_tracker_base(destruction_tracker_base&&) = delete;
  destruction_tracker_base& operator=(destruction_tracker_base&&) = delete;
};

struct destruction_tracker : destruction_tracker_base {
  explicit destruction_tracker(bool* deleted) : deleted(deleted) {}

  ~destruction_tracker() {
    *deleted = true;
  }

private:
  bool* deleted;
};
