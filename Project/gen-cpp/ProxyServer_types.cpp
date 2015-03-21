/**
 * Autogenerated by Thrift Compiler (0.9.1)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#include "ProxyServer_types.h"

#include <algorithm>

namespace HTTP_Server {

const char* HTTPServer::ascii_fingerprint = "5EA2D527ECA3BA20C77AFC023EE8C05F";
const uint8_t HTTPServer::binary_fingerprint[16] = {0x5E,0xA2,0xD5,0x27,0xEC,0xA3,0xBA,0x20,0xC7,0x7A,0xFC,0x02,0x3E,0xE8,0xC0,0x5F};

uint32_t HTTPServer::read(::apache::thrift::protocol::TProtocol* iprot) {

  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;


  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_MAP) {
          {
            this->url_Body.clear();
            uint32_t _size0;
            ::apache::thrift::protocol::TType _ktype1;
            ::apache::thrift::protocol::TType _vtype2;
            xfer += iprot->readMapBegin(_ktype1, _vtype2, _size0);
            uint32_t _i4;
            for (_i4 = 0; _i4 < _size0; ++_i4)
            {
              std::string _key5;
              xfer += iprot->readString(_key5);
              std::string& _val6 = this->url_Body[_key5];
              xfer += iprot->readString(_val6);
            }
            xfer += iprot->readMapEnd();
          }
          this->__isset.url_Body = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  return xfer;
}

uint32_t HTTPServer::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  xfer += oprot->writeStructBegin("HTTPServer");

  xfer += oprot->writeFieldBegin("url_Body", ::apache::thrift::protocol::T_MAP, 1);
  {
    xfer += oprot->writeMapBegin(::apache::thrift::protocol::T_STRING, ::apache::thrift::protocol::T_STRING, static_cast<uint32_t>(this->url_Body.size()));
    std::map<std::string, std::string> ::const_iterator _iter7;
    for (_iter7 = this->url_Body.begin(); _iter7 != this->url_Body.end(); ++_iter7)
    {
      xfer += oprot->writeString(_iter7->first);
      xfer += oprot->writeString(_iter7->second);
    }
    xfer += oprot->writeMapEnd();
  }
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(HTTPServer &a, HTTPServer &b) {
  using ::std::swap;
  swap(a.url_Body, b.url_Body);
  swap(a.__isset, b.__isset);
}

} // namespace