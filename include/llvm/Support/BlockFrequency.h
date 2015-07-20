//===-------- BlockFrequency.h - Block Frequency Wrapper --------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements Block Frequency class.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_SUPPORT_BLOCKFREQUENCY_H
#define LLVM_SUPPORT_BLOCKFREQUENCY_H

#include "llvm/Support/DataTypes.h"

namespace llvm {

class raw_ostream;
class BranchProbability;

// This class represents Block Frequency as a 64-bit value.
class BlockFrequency {

  uint64_t Frequency;
  static const int64_t ENTRY_FREQ = 1 << 14;

  /// \brief Scale the given BlockFrequency by N/D. Return the remainder from
  /// the division by D. Upon overflow, the routine will saturate and
  /// additionally will return the remainder set to D.
  uint32_t scale(uint32_t N, uint32_t D);

public:
  BlockFrequency(uint64_t Freq = 0) : Frequency(Freq) { }

  /// \brief Returns the frequency of the entry block of the function.
  static uint64_t getEntryFrequency() { return ENTRY_FREQ; }

  /// \brief Returns the maximum possible frequency, the saturation value.
  static uint64_t getMaxFrequency() { return -1ULL; }

  /// \brief Returns the frequency as a fixpoint number scaled by the entry
  /// frequency.
  uint64_t getFrequency() const { return Frequency; }

  /// \brief Multiplies with a branch probability. The computation will never
  /// overflow.
  BlockFrequency &operator*=(const BranchProbability &Prob);
  const BlockFrequency operator*(const BranchProbability &Prob) const;

  /// \brief Divide by a non-zero branch probability using saturating
  /// arithmetic.
  BlockFrequency &operator/=(const BranchProbability &Prob);
  BlockFrequency operator/(const BranchProbability &Prob) const;

  /// \brief Adds another block frequency using saturating arithmetic.
  BlockFrequency &operator+=(const BlockFrequency &Freq);
  const BlockFrequency operator+(const BlockFrequency &Freq) const;

  /// \brief Scale the given BlockFrequency by N/D. Return the remainder from
  /// the division by D. Upon overflow, the routine will saturate.
  uint32_t scale(const BranchProbability &Prob);

  bool operator<(const BlockFrequency &RHS) const {
    return Frequency < RHS.Frequency;
  }

  bool operator<=(const BlockFrequency &RHS) const {
    return Frequency <= RHS.Frequency;
  }

  bool operator>(const BlockFrequency &RHS) const {
    return Frequency > RHS.Frequency;
  }

  bool operator>=(const BlockFrequency &RHS) const {
    return Frequency >= RHS.Frequency;
  }

  void print(raw_ostream &OS) const;
};

raw_ostream &operator<<(raw_ostream &OS, const BlockFrequency &Freq);

}

#endif
