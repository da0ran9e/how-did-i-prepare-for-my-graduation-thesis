//
// Generated file, do not edit! Created by nedtool 5.6 from node/communication/routing/gstebRouting/GSTEBRouting.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wshadow"
#  pragma clang diagnostic ignored "-Wconversion"
#  pragma clang diagnostic ignored "-Wunused-parameter"
#  pragma clang diagnostic ignored "-Wc++98-compat"
#  pragma clang diagnostic ignored "-Wunreachable-code-break"
#  pragma clang diagnostic ignored "-Wold-style-cast"
#elif defined(__GNUC__)
#  pragma GCC diagnostic ignored "-Wshadow"
#  pragma GCC diagnostic ignored "-Wconversion"
#  pragma GCC diagnostic ignored "-Wunused-parameter"
#  pragma GCC diagnostic ignored "-Wold-style-cast"
#  pragma GCC diagnostic ignored "-Wsuggest-attribute=noreturn"
#  pragma GCC diagnostic ignored "-Wfloat-conversion"
#endif

#include <iostream>
#include <sstream>
#include "GSTEBRouting_m.h"

namespace omnetpp {

// Template pack/unpack rules. They are declared *after* a1l type-specific pack functions for multiple reasons.
// They are in the omnetpp namespace, to allow them to be found by argument-dependent lookup via the cCommBuffer argument

// Packing/unpacking an std::vector
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::vector<T,A>& v)
{
    int n = v.size();
    doParsimPacking(buffer, n);
    for (int i = 0; i < n; i++)
        doParsimPacking(buffer, v[i]);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::vector<T,A>& v)
{
    int n;
    doParsimUnpacking(buffer, n);
    v.resize(n);
    for (int i = 0; i < n; i++)
        doParsimUnpacking(buffer, v[i]);
}

// Packing/unpacking an std::list
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::list<T,A>& l)
{
    doParsimPacking(buffer, (int)l.size());
    for (typename std::list<T,A>::const_iterator it = l.begin(); it != l.end(); ++it)
        doParsimPacking(buffer, (T&)*it);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::list<T,A>& l)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i=0; i<n; i++) {
        l.push_back(T());
        doParsimUnpacking(buffer, l.back());
    }
}

// Packing/unpacking an std::set
template<typename T, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::set<T,Tr,A>& s)
{
    doParsimPacking(buffer, (int)s.size());
    for (typename std::set<T,Tr,A>::const_iterator it = s.begin(); it != s.end(); ++it)
        doParsimPacking(buffer, *it);
}

template<typename T, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::set<T,Tr,A>& s)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i=0; i<n; i++) {
        T x;
        doParsimUnpacking(buffer, x);
        s.insert(x);
    }
}

// Packing/unpacking an std::map
template<typename K, typename V, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::map<K,V,Tr,A>& m)
{
    doParsimPacking(buffer, (int)m.size());
    for (typename std::map<K,V,Tr,A>::const_iterator it = m.begin(); it != m.end(); ++it) {
        doParsimPacking(buffer, it->first);
        doParsimPacking(buffer, it->second);
    }
}

template<typename K, typename V, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::map<K,V,Tr,A>& m)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i=0; i<n; i++) {
        K k; V v;
        doParsimUnpacking(buffer, k);
        doParsimUnpacking(buffer, v);
        m[k] = v;
    }
}

// Default pack/unpack function for arrays
template<typename T>
void doParsimArrayPacking(omnetpp::cCommBuffer *b, const T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimPacking(b, t[i]);
}

template<typename T>
void doParsimArrayUnpacking(omnetpp::cCommBuffer *b, T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimUnpacking(b, t[i]);
}

// Default rule to prevent compiler from choosing base class' doParsimPacking() function
template<typename T>
void doParsimPacking(omnetpp::cCommBuffer *, const T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimPacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

template<typename T>
void doParsimUnpacking(omnetpp::cCommBuffer *, T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimUnpacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

}  // namespace omnetpp


// forward
template<typename T, typename A>
std::ostream& operator<<(std::ostream& out, const std::vector<T,A>& vec);

// Template rule which fires if a struct or class doesn't have operator<<
template<typename T>
inline std::ostream& operator<<(std::ostream& out,const T&) {return out;}

// operator<< for std::vector<T>
template<typename T, typename A>
inline std::ostream& operator<<(std::ostream& out, const std::vector<T,A>& vec)
{
    out.put('{');
    for(typename std::vector<T,A>::const_iterator it = vec.begin(); it != vec.end(); ++it)
    {
        if (it != vec.begin()) {
            out.put(','); out.put(' ');
        }
        out << *it;
    }
    out.put('}');
    
    char buf[32];
    sprintf(buf, " (size=%u)", (unsigned int)vec.size());
    out.write(buf, strlen(buf));
    return out;
}

EXECUTE_ON_STARTUP(
    omnetpp::cEnum *e = omnetpp::cEnum::find("GSTEBRoutingPacketType");
    if (!e) omnetpp::enums.getInstance()->add(e = new omnetpp::cEnum("GSTEBRoutingPacketType"));
    e->insert(BS_BROADCAST_PACKET, "BS_BROADCAST_PACKET");
    e->insert(SENSOR_BROADCAST_PACKET, "SENSOR_BROADCAST_PACKET");
    e->insert(NEIGHBOR_BROADCAST_PACKET, "NEIGHBOR_BROADCAST_PACKET");
    e->insert(NODE_INFO_PACKET, "NODE_INFO_PACKET");
    e->insert(NODE_CONTROL_PACKET, "NODE_CONTROL_PACKET");
)

GSTEBBSBroadcastInfo::GSTEBBSBroadcastInfo()
{
    this->senderX = 0;
    this->senderY = 0;
    this->x = 0;
    this->y = 0;
    this->timeStart = 0;
    this->numNodes = 0;
    this->sinkId = 0;
    this->timeSlot = 0;
}

void __doPacking(omnetpp::cCommBuffer *b, const GSTEBBSBroadcastInfo& a)
{
    doParsimPacking(b,a.senderX);
    doParsimPacking(b,a.senderY);
    doParsimPacking(b,a.x);
    doParsimPacking(b,a.y);
    doParsimPacking(b,a.timeStart);
    doParsimPacking(b,a.numNodes);
    doParsimPacking(b,a.sinkId);
    doParsimPacking(b,a.timeSlot);
}

void __doUnpacking(omnetpp::cCommBuffer *b, GSTEBBSBroadcastInfo& a)
{
    doParsimUnpacking(b,a.senderX);
    doParsimUnpacking(b,a.senderY);
    doParsimUnpacking(b,a.x);
    doParsimUnpacking(b,a.y);
    doParsimUnpacking(b,a.timeStart);
    doParsimUnpacking(b,a.numNodes);
    doParsimUnpacking(b,a.sinkId);
    doParsimUnpacking(b,a.timeSlot);
}

class GSTEBBSBroadcastInfoDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    GSTEBBSBroadcastInfoDescriptor();
    virtual ~GSTEBBSBroadcastInfoDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyname) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyname) const override;
    virtual int getFieldArraySize(void *object, int field) const override;

    virtual const char *getFieldDynamicTypeString(void *object, int field, int i) const override;
    virtual std::string getFieldValueAsString(void *object, int field, int i) const override;
    virtual bool setFieldValueAsString(void *object, int field, int i, const char *value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual void *getFieldStructValuePointer(void *object, int field, int i) const override;
};

Register_ClassDescriptor(GSTEBBSBroadcastInfoDescriptor)

GSTEBBSBroadcastInfoDescriptor::GSTEBBSBroadcastInfoDescriptor() : omnetpp::cClassDescriptor("GSTEBBSBroadcastInfo", "")
{
    propertynames = nullptr;
}

GSTEBBSBroadcastInfoDescriptor::~GSTEBBSBroadcastInfoDescriptor()
{
    delete[] propertynames;
}

bool GSTEBBSBroadcastInfoDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<GSTEBBSBroadcastInfo *>(obj)!=nullptr;
}

const char **GSTEBBSBroadcastInfoDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *GSTEBBSBroadcastInfoDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int GSTEBBSBroadcastInfoDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 8+basedesc->getFieldCount() : 8;
}

unsigned int GSTEBBSBroadcastInfoDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<8) ? fieldTypeFlags[field] : 0;
}

const char *GSTEBBSBroadcastInfoDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "senderX",
        "senderY",
        "x",
        "y",
        "timeStart",
        "numNodes",
        "sinkId",
        "timeSlot",
    };
    return (field>=0 && field<8) ? fieldNames[field] : nullptr;
}

int GSTEBBSBroadcastInfoDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='s' && strcmp(fieldName, "senderX")==0) return base+0;
    if (fieldName[0]=='s' && strcmp(fieldName, "senderY")==0) return base+1;
    if (fieldName[0]=='x' && strcmp(fieldName, "x")==0) return base+2;
    if (fieldName[0]=='y' && strcmp(fieldName, "y")==0) return base+3;
    if (fieldName[0]=='t' && strcmp(fieldName, "timeStart")==0) return base+4;
    if (fieldName[0]=='n' && strcmp(fieldName, "numNodes")==0) return base+5;
    if (fieldName[0]=='s' && strcmp(fieldName, "sinkId")==0) return base+6;
    if (fieldName[0]=='t' && strcmp(fieldName, "timeSlot")==0) return base+7;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *GSTEBBSBroadcastInfoDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",
        "int",
        "int",
        "int",
        "double",
        "int",
        "int",
        "double",
    };
    return (field>=0 && field<8) ? fieldTypeStrings[field] : nullptr;
}

const char **GSTEBBSBroadcastInfoDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldPropertyNames(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *GSTEBBSBroadcastInfoDescriptor::getFieldProperty(int field, const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldProperty(field, propertyname);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int GSTEBBSBroadcastInfoDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    GSTEBBSBroadcastInfo *pp = (GSTEBBSBroadcastInfo *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

const char *GSTEBBSBroadcastInfoDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    GSTEBBSBroadcastInfo *pp = (GSTEBBSBroadcastInfo *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string GSTEBBSBroadcastInfoDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    GSTEBBSBroadcastInfo *pp = (GSTEBBSBroadcastInfo *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->senderX);
        case 1: return long2string(pp->senderY);
        case 2: return long2string(pp->x);
        case 3: return long2string(pp->y);
        case 4: return double2string(pp->timeStart);
        case 5: return long2string(pp->numNodes);
        case 6: return long2string(pp->sinkId);
        case 7: return double2string(pp->timeSlot);
        default: return "";
    }
}

bool GSTEBBSBroadcastInfoDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    GSTEBBSBroadcastInfo *pp = (GSTEBBSBroadcastInfo *)object; (void)pp;
    switch (field) {
        case 0: pp->senderX = string2long(value); return true;
        case 1: pp->senderY = string2long(value); return true;
        case 2: pp->x = string2long(value); return true;
        case 3: pp->y = string2long(value); return true;
        case 4: pp->timeStart = string2double(value); return true;
        case 5: pp->numNodes = string2long(value); return true;
        case 6: pp->sinkId = string2long(value); return true;
        case 7: pp->timeSlot = string2double(value); return true;
        default: return false;
    }
}

const char *GSTEBBSBroadcastInfoDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

void *GSTEBBSBroadcastInfoDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    GSTEBBSBroadcastInfo *pp = (GSTEBBSBroadcastInfo *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

GSTEBSensorBroadcastInfo::GSTEBSensorBroadcastInfo()
{
    this->nX = 0;
    this->nY = 0;
    this->nEL = 0;
}

void __doPacking(omnetpp::cCommBuffer *b, const GSTEBSensorBroadcastInfo& a)
{
    doParsimPacking(b,a.nX);
    doParsimPacking(b,a.nY);
    doParsimPacking(b,a.nEL);
}

void __doUnpacking(omnetpp::cCommBuffer *b, GSTEBSensorBroadcastInfo& a)
{
    doParsimUnpacking(b,a.nX);
    doParsimUnpacking(b,a.nY);
    doParsimUnpacking(b,a.nEL);
}

class GSTEBSensorBroadcastInfoDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    GSTEBSensorBroadcastInfoDescriptor();
    virtual ~GSTEBSensorBroadcastInfoDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyname) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyname) const override;
    virtual int getFieldArraySize(void *object, int field) const override;

    virtual const char *getFieldDynamicTypeString(void *object, int field, int i) const override;
    virtual std::string getFieldValueAsString(void *object, int field, int i) const override;
    virtual bool setFieldValueAsString(void *object, int field, int i, const char *value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual void *getFieldStructValuePointer(void *object, int field, int i) const override;
};

Register_ClassDescriptor(GSTEBSensorBroadcastInfoDescriptor)

GSTEBSensorBroadcastInfoDescriptor::GSTEBSensorBroadcastInfoDescriptor() : omnetpp::cClassDescriptor("GSTEBSensorBroadcastInfo", "")
{
    propertynames = nullptr;
}

GSTEBSensorBroadcastInfoDescriptor::~GSTEBSensorBroadcastInfoDescriptor()
{
    delete[] propertynames;
}

bool GSTEBSensorBroadcastInfoDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<GSTEBSensorBroadcastInfo *>(obj)!=nullptr;
}

const char **GSTEBSensorBroadcastInfoDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *GSTEBSensorBroadcastInfoDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int GSTEBSensorBroadcastInfoDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 3+basedesc->getFieldCount() : 3;
}

unsigned int GSTEBSensorBroadcastInfoDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<3) ? fieldTypeFlags[field] : 0;
}

const char *GSTEBSensorBroadcastInfoDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "nX",
        "nY",
        "nEL",
    };
    return (field>=0 && field<3) ? fieldNames[field] : nullptr;
}

int GSTEBSensorBroadcastInfoDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='n' && strcmp(fieldName, "nX")==0) return base+0;
    if (fieldName[0]=='n' && strcmp(fieldName, "nY")==0) return base+1;
    if (fieldName[0]=='n' && strcmp(fieldName, "nEL")==0) return base+2;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *GSTEBSensorBroadcastInfoDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",
        "int",
        "double",
    };
    return (field>=0 && field<3) ? fieldTypeStrings[field] : nullptr;
}

const char **GSTEBSensorBroadcastInfoDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldPropertyNames(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *GSTEBSensorBroadcastInfoDescriptor::getFieldProperty(int field, const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldProperty(field, propertyname);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int GSTEBSensorBroadcastInfoDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    GSTEBSensorBroadcastInfo *pp = (GSTEBSensorBroadcastInfo *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

const char *GSTEBSensorBroadcastInfoDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    GSTEBSensorBroadcastInfo *pp = (GSTEBSensorBroadcastInfo *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string GSTEBSensorBroadcastInfoDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    GSTEBSensorBroadcastInfo *pp = (GSTEBSensorBroadcastInfo *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->nX);
        case 1: return long2string(pp->nY);
        case 2: return double2string(pp->nEL);
        default: return "";
    }
}

bool GSTEBSensorBroadcastInfoDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    GSTEBSensorBroadcastInfo *pp = (GSTEBSensorBroadcastInfo *)object; (void)pp;
    switch (field) {
        case 0: pp->nX = string2long(value); return true;
        case 1: pp->nY = string2long(value); return true;
        case 2: pp->nEL = string2double(value); return true;
        default: return false;
    }
}

const char *GSTEBSensorBroadcastInfoDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

void *GSTEBSensorBroadcastInfoDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    GSTEBSensorBroadcastInfo *pp = (GSTEBSensorBroadcastInfo *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

Register_Class(GSTEBRoutingPacket)

GSTEBRoutingPacket::GSTEBRoutingPacket(const char *name, short kind) : ::RoutingPacket(name,kind)
{
    this->packetType = 0;
    this->ttl = 0;
    this->nNumber = 0;
    for (unsigned int i=0; i<500; i++)
        this->nId[i] = 0;
    for (unsigned int i=0; i<500; i++)
        this->nXCoor[i] = 0;
    for (unsigned int i=0; i<500; i++)
        this->nYCoor[i] = 0;
    for (unsigned int i=0; i<500; i++)
        this->nEL[i] = 0;
    this->nnNumber = 0;
    for (unsigned int i=0; i<500; i++)
        this->nNeighbor[i] = 0;
    for (unsigned int i=0; i<500; i++)
        this->nnId[i] = 0;
    for (unsigned int i=0; i<500; i++)
        this->nnXCoor[i] = 0;
    for (unsigned int i=0; i<500; i++)
        this->nnYCoor[i] = 0;
    for (unsigned int i=0; i<500; i++)
        this->nnEL[i] = 0;
    for (unsigned int i=0; i<500; i++)
        this->routingTree[i] = 0;
}

GSTEBRoutingPacket::GSTEBRoutingPacket(const GSTEBRoutingPacket& other) : ::RoutingPacket(other)
{
    copy(other);
}

GSTEBRoutingPacket::~GSTEBRoutingPacket()
{
}

GSTEBRoutingPacket& GSTEBRoutingPacket::operator=(const GSTEBRoutingPacket& other)
{
    if (this==&other) return *this;
    ::RoutingPacket::operator=(other);
    copy(other);
    return *this;
}

void GSTEBRoutingPacket::copy(const GSTEBRoutingPacket& other)
{
    this->packetType = other.packetType;
    this->ttl = other.ttl;
    this->bSBroadcastData = other.bSBroadcastData;
    this->sensorBroadcastData = other.sensorBroadcastData;
    this->nNumber = other.nNumber;
    for (unsigned int i=0; i<500; i++)
        this->nId[i] = other.nId[i];
    for (unsigned int i=0; i<500; i++)
        this->nXCoor[i] = other.nXCoor[i];
    for (unsigned int i=0; i<500; i++)
        this->nYCoor[i] = other.nYCoor[i];
    for (unsigned int i=0; i<500; i++)
        this->nEL[i] = other.nEL[i];
    this->nnNumber = other.nnNumber;
    for (unsigned int i=0; i<500; i++)
        this->nNeighbor[i] = other.nNeighbor[i];
    for (unsigned int i=0; i<500; i++)
        this->nnId[i] = other.nnId[i];
    for (unsigned int i=0; i<500; i++)
        this->nnXCoor[i] = other.nnXCoor[i];
    for (unsigned int i=0; i<500; i++)
        this->nnYCoor[i] = other.nnYCoor[i];
    for (unsigned int i=0; i<500; i++)
        this->nnEL[i] = other.nnEL[i];
    for (unsigned int i=0; i<500; i++)
        this->routingTree[i] = other.routingTree[i];
}

void GSTEBRoutingPacket::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::RoutingPacket::parsimPack(b);
    doParsimPacking(b,this->packetType);
    doParsimPacking(b,this->ttl);
    doParsimPacking(b,this->bSBroadcastData);
    doParsimPacking(b,this->sensorBroadcastData);
    doParsimPacking(b,this->nNumber);
    doParsimArrayPacking(b,this->nId,500);
    doParsimArrayPacking(b,this->nXCoor,500);
    doParsimArrayPacking(b,this->nYCoor,500);
    doParsimArrayPacking(b,this->nEL,500);
    doParsimPacking(b,this->nnNumber);
    doParsimArrayPacking(b,this->nNeighbor,500);
    doParsimArrayPacking(b,this->nnId,500);
    doParsimArrayPacking(b,this->nnXCoor,500);
    doParsimArrayPacking(b,this->nnYCoor,500);
    doParsimArrayPacking(b,this->nnEL,500);
    doParsimArrayPacking(b,this->routingTree,500);
}

void GSTEBRoutingPacket::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::RoutingPacket::parsimUnpack(b);
    doParsimUnpacking(b,this->packetType);
    doParsimUnpacking(b,this->ttl);
    doParsimUnpacking(b,this->bSBroadcastData);
    doParsimUnpacking(b,this->sensorBroadcastData);
    doParsimUnpacking(b,this->nNumber);
    doParsimArrayUnpacking(b,this->nId,500);
    doParsimArrayUnpacking(b,this->nXCoor,500);
    doParsimArrayUnpacking(b,this->nYCoor,500);
    doParsimArrayUnpacking(b,this->nEL,500);
    doParsimUnpacking(b,this->nnNumber);
    doParsimArrayUnpacking(b,this->nNeighbor,500);
    doParsimArrayUnpacking(b,this->nnId,500);
    doParsimArrayUnpacking(b,this->nnXCoor,500);
    doParsimArrayUnpacking(b,this->nnYCoor,500);
    doParsimArrayUnpacking(b,this->nnEL,500);
    doParsimArrayUnpacking(b,this->routingTree,500);
}

int GSTEBRoutingPacket::getPacketType() const
{
    return this->packetType;
}

void GSTEBRoutingPacket::setPacketType(int packetType)
{
    this->packetType = packetType;
}

int GSTEBRoutingPacket::getTtl() const
{
    return this->ttl;
}

void GSTEBRoutingPacket::setTtl(int ttl)
{
    this->ttl = ttl;
}

GSTEBBSBroadcastInfo& GSTEBRoutingPacket::getBSBroadcastData()
{
    return this->bSBroadcastData;
}

void GSTEBRoutingPacket::setBSBroadcastData(const GSTEBBSBroadcastInfo& bSBroadcastData)
{
    this->bSBroadcastData = bSBroadcastData;
}

GSTEBSensorBroadcastInfo& GSTEBRoutingPacket::getSensorBroadcastData()
{
    return this->sensorBroadcastData;
}

void GSTEBRoutingPacket::setSensorBroadcastData(const GSTEBSensorBroadcastInfo& sensorBroadcastData)
{
    this->sensorBroadcastData = sensorBroadcastData;
}

int GSTEBRoutingPacket::getNNumber() const
{
    return this->nNumber;
}

void GSTEBRoutingPacket::setNNumber(int nNumber)
{
    this->nNumber = nNumber;
}

unsigned int GSTEBRoutingPacket::getNIdArraySize() const
{
    return 500;
}

int GSTEBRoutingPacket::getNId(unsigned int k) const
{
    if (k>=500) throw omnetpp::cRuntimeError("Array of size 500 indexed by %lu", (unsigned long)k);
    return this->nId[k];
}

void GSTEBRoutingPacket::setNId(unsigned int k, int nId)
{
    if (k>=500) throw omnetpp::cRuntimeError("Array of size 500 indexed by %lu", (unsigned long)k);
    this->nId[k] = nId;
}

unsigned int GSTEBRoutingPacket::getNXCoorArraySize() const
{
    return 500;
}

int GSTEBRoutingPacket::getNXCoor(unsigned int k) const
{
    if (k>=500) throw omnetpp::cRuntimeError("Array of size 500 indexed by %lu", (unsigned long)k);
    return this->nXCoor[k];
}

void GSTEBRoutingPacket::setNXCoor(unsigned int k, int nXCoor)
{
    if (k>=500) throw omnetpp::cRuntimeError("Array of size 500 indexed by %lu", (unsigned long)k);
    this->nXCoor[k] = nXCoor;
}

unsigned int GSTEBRoutingPacket::getNYCoorArraySize() const
{
    return 500;
}

int GSTEBRoutingPacket::getNYCoor(unsigned int k) const
{
    if (k>=500) throw omnetpp::cRuntimeError("Array of size 500 indexed by %lu", (unsigned long)k);
    return this->nYCoor[k];
}

void GSTEBRoutingPacket::setNYCoor(unsigned int k, int nYCoor)
{
    if (k>=500) throw omnetpp::cRuntimeError("Array of size 500 indexed by %lu", (unsigned long)k);
    this->nYCoor[k] = nYCoor;
}

unsigned int GSTEBRoutingPacket::getNELArraySize() const
{
    return 500;
}

int GSTEBRoutingPacket::getNEL(unsigned int k) const
{
    if (k>=500) throw omnetpp::cRuntimeError("Array of size 500 indexed by %lu", (unsigned long)k);
    return this->nEL[k];
}

void GSTEBRoutingPacket::setNEL(unsigned int k, int nEL)
{
    if (k>=500) throw omnetpp::cRuntimeError("Array of size 500 indexed by %lu", (unsigned long)k);
    this->nEL[k] = nEL;
}

int GSTEBRoutingPacket::getNnNumber() const
{
    return this->nnNumber;
}

void GSTEBRoutingPacket::setNnNumber(int nnNumber)
{
    this->nnNumber = nnNumber;
}

unsigned int GSTEBRoutingPacket::getNNeighborArraySize() const
{
    return 500;
}

int GSTEBRoutingPacket::getNNeighbor(unsigned int k) const
{
    if (k>=500) throw omnetpp::cRuntimeError("Array of size 500 indexed by %lu", (unsigned long)k);
    return this->nNeighbor[k];
}

void GSTEBRoutingPacket::setNNeighbor(unsigned int k, int nNeighbor)
{
    if (k>=500) throw omnetpp::cRuntimeError("Array of size 500 indexed by %lu", (unsigned long)k);
    this->nNeighbor[k] = nNeighbor;
}

unsigned int GSTEBRoutingPacket::getNnIdArraySize() const
{
    return 500;
}

int GSTEBRoutingPacket::getNnId(unsigned int k) const
{
    if (k>=500) throw omnetpp::cRuntimeError("Array of size 500 indexed by %lu", (unsigned long)k);
    return this->nnId[k];
}

void GSTEBRoutingPacket::setNnId(unsigned int k, int nnId)
{
    if (k>=500) throw omnetpp::cRuntimeError("Array of size 500 indexed by %lu", (unsigned long)k);
    this->nnId[k] = nnId;
}

unsigned int GSTEBRoutingPacket::getNnXCoorArraySize() const
{
    return 500;
}

int GSTEBRoutingPacket::getNnXCoor(unsigned int k) const
{
    if (k>=500) throw omnetpp::cRuntimeError("Array of size 500 indexed by %lu", (unsigned long)k);
    return this->nnXCoor[k];
}

void GSTEBRoutingPacket::setNnXCoor(unsigned int k, int nnXCoor)
{
    if (k>=500) throw omnetpp::cRuntimeError("Array of size 500 indexed by %lu", (unsigned long)k);
    this->nnXCoor[k] = nnXCoor;
}

unsigned int GSTEBRoutingPacket::getNnYCoorArraySize() const
{
    return 500;
}

int GSTEBRoutingPacket::getNnYCoor(unsigned int k) const
{
    if (k>=500) throw omnetpp::cRuntimeError("Array of size 500 indexed by %lu", (unsigned long)k);
    return this->nnYCoor[k];
}

void GSTEBRoutingPacket::setNnYCoor(unsigned int k, int nnYCoor)
{
    if (k>=500) throw omnetpp::cRuntimeError("Array of size 500 indexed by %lu", (unsigned long)k);
    this->nnYCoor[k] = nnYCoor;
}

unsigned int GSTEBRoutingPacket::getNnELArraySize() const
{
    return 500;
}

int GSTEBRoutingPacket::getNnEL(unsigned int k) const
{
    if (k>=500) throw omnetpp::cRuntimeError("Array of size 500 indexed by %lu", (unsigned long)k);
    return this->nnEL[k];
}

void GSTEBRoutingPacket::setNnEL(unsigned int k, int nnEL)
{
    if (k>=500) throw omnetpp::cRuntimeError("Array of size 500 indexed by %lu", (unsigned long)k);
    this->nnEL[k] = nnEL;
}

unsigned int GSTEBRoutingPacket::getRoutingTreeArraySize() const
{
    return 500;
}

int GSTEBRoutingPacket::getRoutingTree(unsigned int k) const
{
    if (k>=500) throw omnetpp::cRuntimeError("Array of size 500 indexed by %lu", (unsigned long)k);
    return this->routingTree[k];
}

void GSTEBRoutingPacket::setRoutingTree(unsigned int k, int routingTree)
{
    if (k>=500) throw omnetpp::cRuntimeError("Array of size 500 indexed by %lu", (unsigned long)k);
    this->routingTree[k] = routingTree;
}

class GSTEBRoutingPacketDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    GSTEBRoutingPacketDescriptor();
    virtual ~GSTEBRoutingPacketDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyname) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyname) const override;
    virtual int getFieldArraySize(void *object, int field) const override;

    virtual const char *getFieldDynamicTypeString(void *object, int field, int i) const override;
    virtual std::string getFieldValueAsString(void *object, int field, int i) const override;
    virtual bool setFieldValueAsString(void *object, int field, int i, const char *value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual void *getFieldStructValuePointer(void *object, int field, int i) const override;
};

Register_ClassDescriptor(GSTEBRoutingPacketDescriptor)

GSTEBRoutingPacketDescriptor::GSTEBRoutingPacketDescriptor() : omnetpp::cClassDescriptor("GSTEBRoutingPacket", "RoutingPacket")
{
    propertynames = nullptr;
}

GSTEBRoutingPacketDescriptor::~GSTEBRoutingPacketDescriptor()
{
    delete[] propertynames;
}

bool GSTEBRoutingPacketDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<GSTEBRoutingPacket *>(obj)!=nullptr;
}

const char **GSTEBRoutingPacketDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *GSTEBRoutingPacketDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int GSTEBRoutingPacketDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 16+basedesc->getFieldCount() : 16;
}

unsigned int GSTEBRoutingPacketDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISCOMPOUND,
        FD_ISCOMPOUND,
        FD_ISEDITABLE,
        FD_ISARRAY | FD_ISEDITABLE,
        FD_ISARRAY | FD_ISEDITABLE,
        FD_ISARRAY | FD_ISEDITABLE,
        FD_ISARRAY | FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISARRAY | FD_ISEDITABLE,
        FD_ISARRAY | FD_ISEDITABLE,
        FD_ISARRAY | FD_ISEDITABLE,
        FD_ISARRAY | FD_ISEDITABLE,
        FD_ISARRAY | FD_ISEDITABLE,
        FD_ISARRAY | FD_ISEDITABLE,
    };
    return (field>=0 && field<16) ? fieldTypeFlags[field] : 0;
}

const char *GSTEBRoutingPacketDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "packetType",
        "ttl",
        "bSBroadcastData",
        "sensorBroadcastData",
        "nNumber",
        "nId",
        "nXCoor",
        "nYCoor",
        "nEL",
        "nnNumber",
        "nNeighbor",
        "nnId",
        "nnXCoor",
        "nnYCoor",
        "nnEL",
        "routingTree",
    };
    return (field>=0 && field<16) ? fieldNames[field] : nullptr;
}

int GSTEBRoutingPacketDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='p' && strcmp(fieldName, "packetType")==0) return base+0;
    if (fieldName[0]=='t' && strcmp(fieldName, "ttl")==0) return base+1;
    if (fieldName[0]=='b' && strcmp(fieldName, "bSBroadcastData")==0) return base+2;
    if (fieldName[0]=='s' && strcmp(fieldName, "sensorBroadcastData")==0) return base+3;
    if (fieldName[0]=='n' && strcmp(fieldName, "nNumber")==0) return base+4;
    if (fieldName[0]=='n' && strcmp(fieldName, "nId")==0) return base+5;
    if (fieldName[0]=='n' && strcmp(fieldName, "nXCoor")==0) return base+6;
    if (fieldName[0]=='n' && strcmp(fieldName, "nYCoor")==0) return base+7;
    if (fieldName[0]=='n' && strcmp(fieldName, "nEL")==0) return base+8;
    if (fieldName[0]=='n' && strcmp(fieldName, "nnNumber")==0) return base+9;
    if (fieldName[0]=='n' && strcmp(fieldName, "nNeighbor")==0) return base+10;
    if (fieldName[0]=='n' && strcmp(fieldName, "nnId")==0) return base+11;
    if (fieldName[0]=='n' && strcmp(fieldName, "nnXCoor")==0) return base+12;
    if (fieldName[0]=='n' && strcmp(fieldName, "nnYCoor")==0) return base+13;
    if (fieldName[0]=='n' && strcmp(fieldName, "nnEL")==0) return base+14;
    if (fieldName[0]=='r' && strcmp(fieldName, "routingTree")==0) return base+15;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *GSTEBRoutingPacketDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",
        "int",
        "GSTEBBSBroadcastInfo",
        "GSTEBSensorBroadcastInfo",
        "int",
        "int",
        "int",
        "int",
        "int",
        "int",
        "int",
        "int",
        "int",
        "int",
        "int",
        "int",
    };
    return (field>=0 && field<16) ? fieldTypeStrings[field] : nullptr;
}

const char **GSTEBRoutingPacketDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldPropertyNames(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        case 0: {
            static const char *names[] = { "enum",  nullptr };
            return names;
        }
        default: return nullptr;
    }
}

const char *GSTEBRoutingPacketDescriptor::getFieldProperty(int field, const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldProperty(field, propertyname);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        case 0:
            if (!strcmp(propertyname,"enum")) return "GSTEBRoutingPacketType";
            return nullptr;
        default: return nullptr;
    }
}

int GSTEBRoutingPacketDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    GSTEBRoutingPacket *pp = (GSTEBRoutingPacket *)object; (void)pp;
    switch (field) {
        case 5: return 500;
        case 6: return 500;
        case 7: return 500;
        case 8: return 500;
        case 10: return 500;
        case 11: return 500;
        case 12: return 500;
        case 13: return 500;
        case 14: return 500;
        case 15: return 500;
        default: return 0;
    }
}

const char *GSTEBRoutingPacketDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    GSTEBRoutingPacket *pp = (GSTEBRoutingPacket *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string GSTEBRoutingPacketDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    GSTEBRoutingPacket *pp = (GSTEBRoutingPacket *)object; (void)pp;
    switch (field) {
        case 0: return enum2string(pp->getPacketType(), "GSTEBRoutingPacketType");
        case 1: return long2string(pp->getTtl());
        case 2: {std::stringstream out; out << pp->getBSBroadcastData(); return out.str();}
        case 3: {std::stringstream out; out << pp->getSensorBroadcastData(); return out.str();}
        case 4: return long2string(pp->getNNumber());
        case 5: return long2string(pp->getNId(i));
        case 6: return long2string(pp->getNXCoor(i));
        case 7: return long2string(pp->getNYCoor(i));
        case 8: return long2string(pp->getNEL(i));
        case 9: return long2string(pp->getNnNumber());
        case 10: return long2string(pp->getNNeighbor(i));
        case 11: return long2string(pp->getNnId(i));
        case 12: return long2string(pp->getNnXCoor(i));
        case 13: return long2string(pp->getNnYCoor(i));
        case 14: return long2string(pp->getNnEL(i));
        case 15: return long2string(pp->getRoutingTree(i));
        default: return "";
    }
}

bool GSTEBRoutingPacketDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    GSTEBRoutingPacket *pp = (GSTEBRoutingPacket *)object; (void)pp;
    switch (field) {
        case 0: pp->setPacketType((GSTEBRoutingPacketType)string2enum(value, "GSTEBRoutingPacketType")); return true;
        case 1: pp->setTtl(string2long(value)); return true;
        case 4: pp->setNNumber(string2long(value)); return true;
        case 5: pp->setNId(i,string2long(value)); return true;
        case 6: pp->setNXCoor(i,string2long(value)); return true;
        case 7: pp->setNYCoor(i,string2long(value)); return true;
        case 8: pp->setNEL(i,string2long(value)); return true;
        case 9: pp->setNnNumber(string2long(value)); return true;
        case 10: pp->setNNeighbor(i,string2long(value)); return true;
        case 11: pp->setNnId(i,string2long(value)); return true;
        case 12: pp->setNnXCoor(i,string2long(value)); return true;
        case 13: pp->setNnYCoor(i,string2long(value)); return true;
        case 14: pp->setNnEL(i,string2long(value)); return true;
        case 15: pp->setRoutingTree(i,string2long(value)); return true;
        default: return false;
    }
}

const char *GSTEBRoutingPacketDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        case 2: return omnetpp::opp_typename(typeid(GSTEBBSBroadcastInfo));
        case 3: return omnetpp::opp_typename(typeid(GSTEBSensorBroadcastInfo));
        default: return nullptr;
    };
}

void *GSTEBRoutingPacketDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    GSTEBRoutingPacket *pp = (GSTEBRoutingPacket *)object; (void)pp;
    switch (field) {
        case 2: return (void *)(&pp->getBSBroadcastData()); break;
        case 3: return (void *)(&pp->getSensorBroadcastData()); break;
        default: return nullptr;
    }
}


