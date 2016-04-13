// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: DQMServices/Core/src/ROOTFilePB.proto

#ifndef PROTOBUF_DQMServices_2fCore_2fsrc_2fROOTFilePB_2eproto__INCLUDED
#define PROTOBUF_DQMServices_2fCore_2fsrc_2fROOTFilePB_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 2006000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 2006001 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)

namespace dqmstorepb {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_DQMServices_2fCore_2fsrc_2fROOTFilePB_2eproto();
void protobuf_AssignDesc_DQMServices_2fCore_2fsrc_2fROOTFilePB_2eproto();
void protobuf_ShutdownFile_DQMServices_2fCore_2fsrc_2fROOTFilePB_2eproto();

class ROOTFilePB;
class ROOTFilePB_Histo;

// ===================================================================

class ROOTFilePB_Histo : public ::google::protobuf::Message {
 public:
  ROOTFilePB_Histo();
  virtual ~ROOTFilePB_Histo();

  ROOTFilePB_Histo(const ROOTFilePB_Histo& from);

  inline ROOTFilePB_Histo& operator=(const ROOTFilePB_Histo& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const ROOTFilePB_Histo& default_instance();

  void Swap(ROOTFilePB_Histo* other);

  // implements Message ----------------------------------------------

  ROOTFilePB_Histo* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const ROOTFilePB_Histo& from);
  void MergeFrom(const ROOTFilePB_Histo& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // required string full_pathname = 1;
  inline bool has_full_pathname() const;
  inline void clear_full_pathname();
  static const int kFullPathnameFieldNumber = 1;
  inline const ::std::string& full_pathname() const;
  inline void set_full_pathname(const ::std::string& value);
  inline void set_full_pathname(const char* value);
  inline void set_full_pathname(const char* value, size_t size);
  inline ::std::string* mutable_full_pathname();
  inline ::std::string* release_full_pathname();
  inline void set_allocated_full_pathname(::std::string* full_pathname);

  // required uint32 size = 2;
  inline bool has_size() const;
  inline void clear_size();
  static const int kSizeFieldNumber = 2;
  inline ::google::protobuf::uint32 size() const;
  inline void set_size(::google::protobuf::uint32 value);

  // required bytes streamed_histo = 3;
  inline bool has_streamed_histo() const;
  inline void clear_streamed_histo();
  static const int kStreamedHistoFieldNumber = 3;
  inline const ::std::string& streamed_histo() const;
  inline void set_streamed_histo(const ::std::string& value);
  inline void set_streamed_histo(const char* value);
  inline void set_streamed_histo(const void* value, size_t size);
  inline ::std::string* mutable_streamed_histo();
  inline ::std::string* release_streamed_histo();
  inline void set_allocated_streamed_histo(::std::string* streamed_histo);

  // required uint32 flags = 4;
  inline bool has_flags() const;
  inline void clear_flags();
  static const int kFlagsFieldNumber = 4;
  inline ::google::protobuf::uint32 flags() const;
  inline void set_flags(::google::protobuf::uint32 value);

  // @@protoc_insertion_point(class_scope:dqmstorepb.ROOTFilePB.Histo)
 private:
  inline void set_has_full_pathname();
  inline void clear_has_full_pathname();
  inline void set_has_size();
  inline void clear_has_size();
  inline void set_has_streamed_histo();
  inline void clear_has_streamed_histo();
  inline void set_has_flags();
  inline void clear_has_flags();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  ::std::string* full_pathname_;
  ::std::string* streamed_histo_;
  ::google::protobuf::uint32 size_;
  ::google::protobuf::uint32 flags_;
  friend void  protobuf_AddDesc_DQMServices_2fCore_2fsrc_2fROOTFilePB_2eproto();
  friend void protobuf_AssignDesc_DQMServices_2fCore_2fsrc_2fROOTFilePB_2eproto();
  friend void protobuf_ShutdownFile_DQMServices_2fCore_2fsrc_2fROOTFilePB_2eproto();

  void InitAsDefaultInstance();
  static ROOTFilePB_Histo* default_instance_;
};
// -------------------------------------------------------------------

class ROOTFilePB : public ::google::protobuf::Message {
 public:
  ROOTFilePB();
  virtual ~ROOTFilePB();

  ROOTFilePB(const ROOTFilePB& from);

  inline ROOTFilePB& operator=(const ROOTFilePB& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const ROOTFilePB& default_instance();

  void Swap(ROOTFilePB* other);

  // implements Message ----------------------------------------------

  ROOTFilePB* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const ROOTFilePB& from);
  void MergeFrom(const ROOTFilePB& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  typedef ROOTFilePB_Histo Histo;

  // accessors -------------------------------------------------------

  // repeated .dqmstorepb.ROOTFilePB.Histo histo = 1;
  inline int histo_size() const;
  inline void clear_histo();
  static const int kHistoFieldNumber = 1;
  inline const ::dqmstorepb::ROOTFilePB_Histo& histo(int index) const;
  inline ::dqmstorepb::ROOTFilePB_Histo* mutable_histo(int index);
  inline ::dqmstorepb::ROOTFilePB_Histo* add_histo();
  inline const ::google::protobuf::RepeatedPtrField< ::dqmstorepb::ROOTFilePB_Histo >&
      histo() const;
  inline ::google::protobuf::RepeatedPtrField< ::dqmstorepb::ROOTFilePB_Histo >*
      mutable_histo();

  // @@protoc_insertion_point(class_scope:dqmstorepb.ROOTFilePB)
 private:

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  ::google::protobuf::RepeatedPtrField< ::dqmstorepb::ROOTFilePB_Histo > histo_;
  friend void  protobuf_AddDesc_DQMServices_2fCore_2fsrc_2fROOTFilePB_2eproto();
  friend void protobuf_AssignDesc_DQMServices_2fCore_2fsrc_2fROOTFilePB_2eproto();
  friend void protobuf_ShutdownFile_DQMServices_2fCore_2fsrc_2fROOTFilePB_2eproto();

  void InitAsDefaultInstance();
  static ROOTFilePB* default_instance_;
};
// ===================================================================


// ===================================================================

// ROOTFilePB_Histo

// required string full_pathname = 1;
inline bool ROOTFilePB_Histo::has_full_pathname() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void ROOTFilePB_Histo::set_has_full_pathname() {
  _has_bits_[0] |= 0x00000001u;
}
inline void ROOTFilePB_Histo::clear_has_full_pathname() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void ROOTFilePB_Histo::clear_full_pathname() {
  if (full_pathname_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    full_pathname_->clear();
  }
  clear_has_full_pathname();
}
inline const ::std::string& ROOTFilePB_Histo::full_pathname() const {
  // @@protoc_insertion_point(field_get:dqmstorepb.ROOTFilePB.Histo.full_pathname)
  return *full_pathname_;
}
inline void ROOTFilePB_Histo::set_full_pathname(const ::std::string& value) {
  set_has_full_pathname();
  if (full_pathname_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    full_pathname_ = new ::std::string;
  }
  full_pathname_->assign(value);
  // @@protoc_insertion_point(field_set:dqmstorepb.ROOTFilePB.Histo.full_pathname)
}
inline void ROOTFilePB_Histo::set_full_pathname(const char* value) {
  set_has_full_pathname();
  if (full_pathname_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    full_pathname_ = new ::std::string;
  }
  full_pathname_->assign(value);
  // @@protoc_insertion_point(field_set_char:dqmstorepb.ROOTFilePB.Histo.full_pathname)
}
inline void ROOTFilePB_Histo::set_full_pathname(const char* value, size_t size) {
  set_has_full_pathname();
  if (full_pathname_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    full_pathname_ = new ::std::string;
  }
  full_pathname_->assign(reinterpret_cast<const char*>(value), size);
  // @@protoc_insertion_point(field_set_pointer:dqmstorepb.ROOTFilePB.Histo.full_pathname)
}
inline ::std::string* ROOTFilePB_Histo::mutable_full_pathname() {
  set_has_full_pathname();
  if (full_pathname_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    full_pathname_ = new ::std::string;
  }
  // @@protoc_insertion_point(field_mutable:dqmstorepb.ROOTFilePB.Histo.full_pathname)
  return full_pathname_;
}
inline ::std::string* ROOTFilePB_Histo::release_full_pathname() {
  clear_has_full_pathname();
  if (full_pathname_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    return NULL;
  } else {
    ::std::string* temp = full_pathname_;
    full_pathname_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
    return temp;
  }
}
inline void ROOTFilePB_Histo::set_allocated_full_pathname(::std::string* full_pathname) {
  if (full_pathname_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    delete full_pathname_;
  }
  if (full_pathname) {
    set_has_full_pathname();
    full_pathname_ = full_pathname;
  } else {
    clear_has_full_pathname();
    full_pathname_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  }
  // @@protoc_insertion_point(field_set_allocated:dqmstorepb.ROOTFilePB.Histo.full_pathname)
}

// required uint32 size = 2;
inline bool ROOTFilePB_Histo::has_size() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void ROOTFilePB_Histo::set_has_size() {
  _has_bits_[0] |= 0x00000002u;
}
inline void ROOTFilePB_Histo::clear_has_size() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void ROOTFilePB_Histo::clear_size() {
  size_ = 0u;
  clear_has_size();
}
inline ::google::protobuf::uint32 ROOTFilePB_Histo::size() const {
  // @@protoc_insertion_point(field_get:dqmstorepb.ROOTFilePB.Histo.size)
  return size_;
}
inline void ROOTFilePB_Histo::set_size(::google::protobuf::uint32 value) {
  set_has_size();
  size_ = value;
  // @@protoc_insertion_point(field_set:dqmstorepb.ROOTFilePB.Histo.size)
}

// required bytes streamed_histo = 3;
inline bool ROOTFilePB_Histo::has_streamed_histo() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void ROOTFilePB_Histo::set_has_streamed_histo() {
  _has_bits_[0] |= 0x00000004u;
}
inline void ROOTFilePB_Histo::clear_has_streamed_histo() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void ROOTFilePB_Histo::clear_streamed_histo() {
  if (streamed_histo_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    streamed_histo_->clear();
  }
  clear_has_streamed_histo();
}
inline const ::std::string& ROOTFilePB_Histo::streamed_histo() const {
  // @@protoc_insertion_point(field_get:dqmstorepb.ROOTFilePB.Histo.streamed_histo)
  return *streamed_histo_;
}
inline void ROOTFilePB_Histo::set_streamed_histo(const ::std::string& value) {
  set_has_streamed_histo();
  if (streamed_histo_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    streamed_histo_ = new ::std::string;
  }
  streamed_histo_->assign(value);
  // @@protoc_insertion_point(field_set:dqmstorepb.ROOTFilePB.Histo.streamed_histo)
}
inline void ROOTFilePB_Histo::set_streamed_histo(const char* value) {
  set_has_streamed_histo();
  if (streamed_histo_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    streamed_histo_ = new ::std::string;
  }
  streamed_histo_->assign(value);
  // @@protoc_insertion_point(field_set_char:dqmstorepb.ROOTFilePB.Histo.streamed_histo)
}
inline void ROOTFilePB_Histo::set_streamed_histo(const void* value, size_t size) {
  set_has_streamed_histo();
  if (streamed_histo_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    streamed_histo_ = new ::std::string;
  }
  streamed_histo_->assign(reinterpret_cast<const char*>(value), size);
  // @@protoc_insertion_point(field_set_pointer:dqmstorepb.ROOTFilePB.Histo.streamed_histo)
}
inline ::std::string* ROOTFilePB_Histo::mutable_streamed_histo() {
  set_has_streamed_histo();
  if (streamed_histo_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    streamed_histo_ = new ::std::string;
  }
  // @@protoc_insertion_point(field_mutable:dqmstorepb.ROOTFilePB.Histo.streamed_histo)
  return streamed_histo_;
}
inline ::std::string* ROOTFilePB_Histo::release_streamed_histo() {
  clear_has_streamed_histo();
  if (streamed_histo_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    return NULL;
  } else {
    ::std::string* temp = streamed_histo_;
    streamed_histo_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
    return temp;
  }
}
inline void ROOTFilePB_Histo::set_allocated_streamed_histo(::std::string* streamed_histo) {
  if (streamed_histo_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    delete streamed_histo_;
  }
  if (streamed_histo) {
    set_has_streamed_histo();
    streamed_histo_ = streamed_histo;
  } else {
    clear_has_streamed_histo();
    streamed_histo_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  }
  // @@protoc_insertion_point(field_set_allocated:dqmstorepb.ROOTFilePB.Histo.streamed_histo)
}

// required uint32 flags = 4;
inline bool ROOTFilePB_Histo::has_flags() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void ROOTFilePB_Histo::set_has_flags() {
  _has_bits_[0] |= 0x00000008u;
}
inline void ROOTFilePB_Histo::clear_has_flags() {
  _has_bits_[0] &= ~0x00000008u;
}
inline void ROOTFilePB_Histo::clear_flags() {
  flags_ = 0u;
  clear_has_flags();
}
inline ::google::protobuf::uint32 ROOTFilePB_Histo::flags() const {
  // @@protoc_insertion_point(field_get:dqmstorepb.ROOTFilePB.Histo.flags)
  return flags_;
}
inline void ROOTFilePB_Histo::set_flags(::google::protobuf::uint32 value) {
  set_has_flags();
  flags_ = value;
  // @@protoc_insertion_point(field_set:dqmstorepb.ROOTFilePB.Histo.flags)
}

// -------------------------------------------------------------------

// ROOTFilePB

// repeated .dqmstorepb.ROOTFilePB.Histo histo = 1;
inline int ROOTFilePB::histo_size() const {
  return histo_.size();
}
inline void ROOTFilePB::clear_histo() {
  histo_.Clear();
}
inline const ::dqmstorepb::ROOTFilePB_Histo& ROOTFilePB::histo(int index) const {
  // @@protoc_insertion_point(field_get:dqmstorepb.ROOTFilePB.histo)
  return histo_.Get(index);
}
inline ::dqmstorepb::ROOTFilePB_Histo* ROOTFilePB::mutable_histo(int index) {
  // @@protoc_insertion_point(field_mutable:dqmstorepb.ROOTFilePB.histo)
  return histo_.Mutable(index);
}
inline ::dqmstorepb::ROOTFilePB_Histo* ROOTFilePB::add_histo() {
  // @@protoc_insertion_point(field_add:dqmstorepb.ROOTFilePB.histo)
  return histo_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::dqmstorepb::ROOTFilePB_Histo >&
ROOTFilePB::histo() const {
  // @@protoc_insertion_point(field_list:dqmstorepb.ROOTFilePB.histo)
  return histo_;
}
inline ::google::protobuf::RepeatedPtrField< ::dqmstorepb::ROOTFilePB_Histo >*
ROOTFilePB::mutable_histo() {
  // @@protoc_insertion_point(field_mutable_list:dqmstorepb.ROOTFilePB.histo)
  return &histo_;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace dqmstorepb

#ifndef SWIG
namespace google {
namespace protobuf {


}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_DQMServices_2fCore_2fsrc_2fROOTFilePB_2eproto__INCLUDED
