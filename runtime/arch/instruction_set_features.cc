/*
 * Copyright (C) 2011 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "instruction_set_features.h"

#include "android-base/strings.h"

#include "base/casts.h"
#include "utils.h"


#include "arm/instruction_set_features_arm.h"
#include "arm64/instruction_set_features_arm64.h"
#include "mips/instruction_set_features_mips.h"
#include "mips64/instruction_set_features_mips64.h"
#include "x86/instruction_set_features_x86.h"
#include "x86_64/instruction_set_features_x86_64.h"

namespace art {

std::unique_ptr<const InstructionSetFeatures> InstructionSetFeatures::FromVariant(
    InstructionSet isa, const std::string& variant, std::string* error_msg) {
  switch (isa) {
    case kArm:
    case kThumb2:
      return ArmInstructionSetFeatures::FromVariant(variant, error_msg);
    case kArm64:
      return Arm64InstructionSetFeatures::FromVariant(variant, error_msg);
    case kMips:
      return MipsInstructionSetFeatures::FromVariant(variant, error_msg);
    case kMips64:
      return Mips64InstructionSetFeatures::FromVariant(variant, error_msg);
    case kX86:
      return X86InstructionSetFeatures::FromVariant(variant, error_msg);
    case kX86_64:
      return X86_64InstructionSetFeatures::FromVariant(variant, error_msg);

    case kNone:
      break;
  }
  UNIMPLEMENTED(FATAL) << isa;
  UNREACHABLE();
}

std::unique_ptr<const InstructionSetFeatures> InstructionSetFeatures::FromBitmap(InstructionSet isa,
                                                                                 uint32_t bitmap) {
  std::unique_ptr<const InstructionSetFeatures> result;
  switch (isa) {
    case kArm:
    case kThumb2:
      result = ArmInstructionSetFeatures::FromBitmap(bitmap);
      break;
    case kArm64:
      result = Arm64InstructionSetFeatures::FromBitmap(bitmap);
      break;
    case kMips:
      result = MipsInstructionSetFeatures::FromBitmap(bitmap);
      break;
    case kMips64:
      result = Mips64InstructionSetFeatures::FromBitmap(bitmap);
      break;
    case kX86:
      result = X86InstructionSetFeatures::FromBitmap(bitmap);
      break;
    case kX86_64:
      result = X86_64InstructionSetFeatures::FromBitmap(bitmap);
      break;

    case kNone:
    default:
      UNIMPLEMENTED(FATAL) << isa;
      UNREACHABLE();
  }
  CHECK_EQ(bitmap, result->AsBitmap());
  return result;
}

std::unique_ptr<const InstructionSetFeatures> InstructionSetFeatures::FromCppDefines() {
  switch (kRuntimeISA) {
    case kArm:
    case kThumb2:
      return ArmInstructionSetFeatures::FromCppDefines();
    case kArm64:
      return Arm64InstructionSetFeatures::FromCppDefines();
    case kMips:
      return MipsInstructionSetFeatures::FromCppDefines();
    case kMips64:
      return Mips64InstructionSetFeatures::FromCppDefines();
    case kX86:
      return X86InstructionSetFeatures::FromCppDefines();
    case kX86_64:
      return X86_64InstructionSetFeatures::FromCppDefines();

    case kNone:
      break;
  }
  UNIMPLEMENTED(FATAL) << kRuntimeISA;
  UNREACHABLE();
}


std::unique_ptr<const InstructionSetFeatures> InstructionSetFeatures::FromCpuInfo() {
  switch (kRuntimeISA) {
    case kArm:
    case kThumb2:
      return ArmInstructionSetFeatures::FromCpuInfo();
    case kArm64:
      return Arm64InstructionSetFeatures::FromCpuInfo();
    case kMips:
      return MipsInstructionSetFeatures::FromCpuInfo();
    case kMips64:
      return Mips64InstructionSetFeatures::FromCpuInfo();
    case kX86:
      return X86InstructionSetFeatures::FromCpuInfo();
    case kX86_64:
      return X86_64InstructionSetFeatures::FromCpuInfo();

    case kNone:
      break;
  }
  UNIMPLEMENTED(FATAL) << kRuntimeISA;
  UNREACHABLE();
}

std::unique_ptr<const InstructionSetFeatures> InstructionSetFeatures::FromHwcap() {
  switch (kRuntimeISA) {
    case kArm:
    case kThumb2:
      return ArmInstructionSetFeatures::FromHwcap();
    case kArm64:
      return Arm64InstructionSetFeatures::FromHwcap();
    case kMips:
      return MipsInstructionSetFeatures::FromHwcap();
    case kMips64:
      return Mips64InstructionSetFeatures::FromHwcap();
    case kX86:
      return X86InstructionSetFeatures::FromHwcap();
    case kX86_64:
      return X86_64InstructionSetFeatures::FromHwcap();

    case kNone:
      break;
  }
  UNIMPLEMENTED(FATAL) << kRuntimeISA;
  UNREACHABLE();
}

std::unique_ptr<const InstructionSetFeatures> InstructionSetFeatures::FromAssembly() {
  switch (kRuntimeISA) {
    case kArm:
    case kThumb2:
      return ArmInstructionSetFeatures::FromAssembly();
    case kArm64:
      return Arm64InstructionSetFeatures::FromAssembly();
    case kMips:
      return MipsInstructionSetFeatures::FromAssembly();
    case kMips64:
      return Mips64InstructionSetFeatures::FromAssembly();
    case kX86:
      return X86InstructionSetFeatures::FromAssembly();
    case kX86_64:
      return X86_64InstructionSetFeatures::FromAssembly();

    case kNone:
      break;
  }
  UNIMPLEMENTED(FATAL) << kRuntimeISA;
  UNREACHABLE();
}

std::unique_ptr<const InstructionSetFeatures> InstructionSetFeatures::AddFeaturesFromString(
    const std::string& feature_list, std::string* error_msg) const {
  if (feature_list.empty()) {
    *error_msg = "No instruction set features specified";
    return std::unique_ptr<const InstructionSetFeatures>();
  }
  std::vector<std::string> features;
  Split(feature_list, ',', &features);
  bool use_default = false;  // Have we seen the 'default' feature?
  bool first = false;  // Is this first feature?
  for (auto it = features.begin(); it != features.end();) {
    if (use_default) {
      *error_msg = "Unexpected instruction set features after 'default'";
      return std::unique_ptr<const InstructionSetFeatures>();
    }
    std::string feature = android::base::Trim(*it);
    bool erase = false;
    if (feature == "default") {
      if (!first) {
        use_default = true;
        erase = true;
      } else {
        *error_msg = "Unexpected instruction set features before 'default'";
        return std::unique_ptr<const InstructionSetFeatures>();
      }
    }
    if (!erase) {
      ++it;
    } else {
      it = features.erase(it);
    }
    first = true;
  }
  // Expectation: "default" is standalone, no other flags. But an empty features vector after
  // processing can also come along if the handled flags are the only ones in the list. So
  // logically, we check "default -> features.empty."
  DCHECK(!use_default || features.empty());

  return AddFeaturesFromSplitString(features, error_msg);
}

const ArmInstructionSetFeatures* InstructionSetFeatures::AsArmInstructionSetFeatures() const {
  DCHECK_EQ(kArm, GetInstructionSet());
  return down_cast<const ArmInstructionSetFeatures*>(this);
}

const Arm64InstructionSetFeatures* InstructionSetFeatures::AsArm64InstructionSetFeatures() const {
  DCHECK_EQ(kArm64, GetInstructionSet());
  return down_cast<const Arm64InstructionSetFeatures*>(this);
}

const MipsInstructionSetFeatures* InstructionSetFeatures::AsMipsInstructionSetFeatures() const {
  DCHECK_EQ(kMips, GetInstructionSet());
  return down_cast<const MipsInstructionSetFeatures*>(this);
}

const Mips64InstructionSetFeatures* InstructionSetFeatures::AsMips64InstructionSetFeatures() const {
  DCHECK_EQ(kMips64, GetInstructionSet());
  return down_cast<const Mips64InstructionSetFeatures*>(this);
}

const X86InstructionSetFeatures* InstructionSetFeatures::AsX86InstructionSetFeatures() const {
  DCHECK(kX86 == GetInstructionSet() || kX86_64 == GetInstructionSet());
  return down_cast<const X86InstructionSetFeatures*>(this);
}

const X86_64InstructionSetFeatures* InstructionSetFeatures::AsX86_64InstructionSetFeatures() const {
  DCHECK_EQ(kX86_64, GetInstructionSet());
  return down_cast<const X86_64InstructionSetFeatures*>(this);
}

bool InstructionSetFeatures::FindVariantInArray(const char* const variants[], size_t num_variants,
                                                const std::string& variant) {
  const char* const * begin = variants;
  const char* const * end = begin + num_variants;
  return std::find(begin, end, variant) != end;
}

std::ostream& operator<<(std::ostream& os, const InstructionSetFeatures& rhs) {
  os << "ISA: " << rhs.GetInstructionSet() << " Feature string: " << rhs.GetFeatureString();
  return os;
}

}  // namespace art
