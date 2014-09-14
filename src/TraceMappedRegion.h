/* -*- Mode: C++; tab-width: 8; c-basic-offset: 2; indent-tabs-mode: nil; -*- */

#ifndef RR_TRACE_MAPPED_REGION_H_
#define RR_TRACE_MAPPED_REGION_H_

#include <stdint.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "remote_ptr.h"
#include "TraceFrame.h"

class TraceMappedRegion {
public:
  TraceMappedRegion(const char* filename, const struct stat& stat,
                    remote_ptr<void> start, remote_ptr<void> end,
                    bool copied = false)
      : stat_(stat), start_(start), end_(end), copied_(copied) {
    strncpy(this->filename, filename, sizeof(this->filename));
    this->filename[sizeof(this->filename) - 1] = 0;
  }
  TraceMappedRegion() : start_(nullptr), end_(nullptr), copied_(false) {
    filename[0] = 0;
    memset(&stat_, 0, sizeof(stat_));
  }

  const char* file_name() const { return filename; }
  const struct stat& stat() const { return stat_; }
  remote_ptr<void> start() const { return start_; }
  remote_ptr<void> end() const { return end_; }
  bool copied() const { return copied_; }

  size_t size() {
    int64_t s = end().as_int() - start().as_int();
    assert(s >= 0);
    return s;
  }

private:
  friend TraceIfstream& operator>>(TraceIfstream& tif, TraceMappedRegion& map);

  char filename[PATH_MAX];
  struct stat stat_;

  /* Bounds of mapped region. */
  remote_ptr<void> start_;
  remote_ptr<void> end_;

  /* Did we save a copy of the mapped region in the trace
   * data? */
  bool copied_;
};

#endif /* RR_TRACE_MAPPED_REGION_H_ */