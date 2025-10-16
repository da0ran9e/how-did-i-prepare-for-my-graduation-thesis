//
// Generated file, do not edit! Created by nedtool 5.6 from node/communication/routing/ssCellularRouting/SSCellularRouting.msg.
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
#include "SSCellularRouting_m.h"

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
    omnetpp::cEnum *e = omnetpp::cEnum::find("SSCellularRoutingPacketType");
    if (!e) omnetpp::enums.getInstance()->add(e = new omnetpp::cEnum("SSCellularRoutingPacketType"));
    e->insert(HELLO_PACKET, "HELLO_PACKET");
    e->insert(CL_ANNOUNCEMENT, "CL_ANNOUNCEMENT");
    e->insert(CL_CONFIRMATION, "CL_CONFIRMATION");
    e->insert(BS_UPDATE_PACKET, "BS_UPDATE_PACKET");
    e->insert(LINK_REQUEST, "LINK_REQUEST");
    e->insert(LINK_ACK, "LINK_ACK");
    e->insert(LINK_ESTABLISHED, "LINK_ESTABLISHED");
    e->insert(INTRA_CELL_ROUTING_UPDATE, "INTRA_CELL_ROUTING_UPDATE");
    e->insert(CL_COMMAND_PACKET, "CL_COMMAND_PACKET");
    e->insert(NCL_CONFIRM_PACKET, "NCL_CONFIRM_PACKET");
    e->insert(ROUTING_TREE_UPDATE_PACKET, "ROUTING_TREE_UPDATE_PACKET");
    e->insert(CH_ANNOUNCEMENT_PACKET, "CH_ANNOUNCEMENT_PACKET");
    e->insert(DATA_PACKET, "DATA_PACKET");
    e->insert(ANNOUNCE_CELL_HOP, "ANNOUNCE_CELL_HOP");
    e->insert(SENSOR_DATA, "SENSOR_DATA");
)

SSCHAnnouncementInfo::SSCHAnnouncementInfo()
{
    this->chId = 0;
}

void __doPacking(omnetpp::cCommBuffer *b, const SSCHAnnouncementInfo& a)
{
    doParsimPacking(b,a.chId);
}

void __doUnpacking(omnetpp::cCommBuffer *b, SSCHAnnouncementInfo& a)
{
    doParsimUnpacking(b,a.chId);
}

class SSCHAnnouncementInfoDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    SSCHAnnouncementInfoDescriptor();
    virtual ~SSCHAnnouncementInfoDescriptor();

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

Register_ClassDescriptor(SSCHAnnouncementInfoDescriptor)

SSCHAnnouncementInfoDescriptor::SSCHAnnouncementInfoDescriptor() : omnetpp::cClassDescriptor("SSCHAnnouncementInfo", "")
{
    propertynames = nullptr;
}

SSCHAnnouncementInfoDescriptor::~SSCHAnnouncementInfoDescriptor()
{
    delete[] propertynames;
}

bool SSCHAnnouncementInfoDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<SSCHAnnouncementInfo *>(obj)!=nullptr;
}

const char **SSCHAnnouncementInfoDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *SSCHAnnouncementInfoDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int SSCHAnnouncementInfoDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 1+basedesc->getFieldCount() : 1;
}

unsigned int SSCHAnnouncementInfoDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
    };
    return (field>=0 && field<1) ? fieldTypeFlags[field] : 0;
}

const char *SSCHAnnouncementInfoDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "chId",
    };
    return (field>=0 && field<1) ? fieldNames[field] : nullptr;
}

int SSCHAnnouncementInfoDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='c' && strcmp(fieldName, "chId")==0) return base+0;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *SSCHAnnouncementInfoDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",
    };
    return (field>=0 && field<1) ? fieldTypeStrings[field] : nullptr;
}

const char **SSCHAnnouncementInfoDescriptor::getFieldPropertyNames(int field) const
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

const char *SSCHAnnouncementInfoDescriptor::getFieldProperty(int field, const char *propertyname) const
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

int SSCHAnnouncementInfoDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    SSCHAnnouncementInfo *pp = (SSCHAnnouncementInfo *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

const char *SSCHAnnouncementInfoDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    SSCHAnnouncementInfo *pp = (SSCHAnnouncementInfo *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string SSCHAnnouncementInfoDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    SSCHAnnouncementInfo *pp = (SSCHAnnouncementInfo *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->chId);
        default: return "";
    }
}

bool SSCHAnnouncementInfoDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    SSCHAnnouncementInfo *pp = (SSCHAnnouncementInfo *)object; (void)pp;
    switch (field) {
        case 0: pp->chId = string2long(value); return true;
        default: return false;
    }
}

const char *SSCHAnnouncementInfoDescriptor::getFieldStructName(int field) const
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

void *SSCHAnnouncementInfoDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    SSCHAnnouncementInfo *pp = (SSCHAnnouncementInfo *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

SSCellHopAnnouncementInfo::SSCellHopAnnouncementInfo()
{
    this->nextCell = 0;
    for (unsigned int i=0; i<1000; i++)
        this->cellPath[i] = 0;
}

void __doPacking(omnetpp::cCommBuffer *b, const SSCellHopAnnouncementInfo& a)
{
    doParsimPacking(b,a.nextCell);
    doParsimArrayPacking(b,a.cellPath,1000);
}

void __doUnpacking(omnetpp::cCommBuffer *b, SSCellHopAnnouncementInfo& a)
{
    doParsimUnpacking(b,a.nextCell);
    doParsimArrayUnpacking(b,a.cellPath,1000);
}

class SSCellHopAnnouncementInfoDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    SSCellHopAnnouncementInfoDescriptor();
    virtual ~SSCellHopAnnouncementInfoDescriptor();

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

Register_ClassDescriptor(SSCellHopAnnouncementInfoDescriptor)

SSCellHopAnnouncementInfoDescriptor::SSCellHopAnnouncementInfoDescriptor() : omnetpp::cClassDescriptor("SSCellHopAnnouncementInfo", "")
{
    propertynames = nullptr;
}

SSCellHopAnnouncementInfoDescriptor::~SSCellHopAnnouncementInfoDescriptor()
{
    delete[] propertynames;
}

bool SSCellHopAnnouncementInfoDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<SSCellHopAnnouncementInfo *>(obj)!=nullptr;
}

const char **SSCellHopAnnouncementInfoDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *SSCellHopAnnouncementInfoDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int SSCellHopAnnouncementInfoDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 2+basedesc->getFieldCount() : 2;
}

unsigned int SSCellHopAnnouncementInfoDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISARRAY | FD_ISEDITABLE,
    };
    return (field>=0 && field<2) ? fieldTypeFlags[field] : 0;
}

const char *SSCellHopAnnouncementInfoDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "nextCell",
        "cellPath",
    };
    return (field>=0 && field<2) ? fieldNames[field] : nullptr;
}

int SSCellHopAnnouncementInfoDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='n' && strcmp(fieldName, "nextCell")==0) return base+0;
    if (fieldName[0]=='c' && strcmp(fieldName, "cellPath")==0) return base+1;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *SSCellHopAnnouncementInfoDescriptor::getFieldTypeString(int field) const
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
    };
    return (field>=0 && field<2) ? fieldTypeStrings[field] : nullptr;
}

const char **SSCellHopAnnouncementInfoDescriptor::getFieldPropertyNames(int field) const
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

const char *SSCellHopAnnouncementInfoDescriptor::getFieldProperty(int field, const char *propertyname) const
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

int SSCellHopAnnouncementInfoDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    SSCellHopAnnouncementInfo *pp = (SSCellHopAnnouncementInfo *)object; (void)pp;
    switch (field) {
        case 1: return 1000;
        default: return 0;
    }
}

const char *SSCellHopAnnouncementInfoDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    SSCellHopAnnouncementInfo *pp = (SSCellHopAnnouncementInfo *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string SSCellHopAnnouncementInfoDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    SSCellHopAnnouncementInfo *pp = (SSCellHopAnnouncementInfo *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->nextCell);
        case 1: if (i>=1000) return "";
                return long2string(pp->cellPath[i]);
        default: return "";
    }
}

bool SSCellHopAnnouncementInfoDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    SSCellHopAnnouncementInfo *pp = (SSCellHopAnnouncementInfo *)object; (void)pp;
    switch (field) {
        case 0: pp->nextCell = string2long(value); return true;
        case 1: if (i>=1000) return false;
                pp->cellPath[i] = string2long(value); return true;
        default: return false;
    }
}

const char *SSCellHopAnnouncementInfoDescriptor::getFieldStructName(int field) const
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

void *SSCellHopAnnouncementInfoDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    SSCellHopAnnouncementInfo *pp = (SSCellHopAnnouncementInfo *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

SSSensorInfo::SSSensorInfo()
{
    this->destinationCH = 0;
    this->dataId = 0;
    this->sensorId = 0;
    this->hopCount = 0;
}

void __doPacking(omnetpp::cCommBuffer *b, const SSSensorInfo& a)
{
    doParsimPacking(b,a.destinationCH);
    doParsimPacking(b,a.dataId);
    doParsimPacking(b,a.sensorId);
    doParsimPacking(b,a.hopCount);
}

void __doUnpacking(omnetpp::cCommBuffer *b, SSSensorInfo& a)
{
    doParsimUnpacking(b,a.destinationCH);
    doParsimUnpacking(b,a.dataId);
    doParsimUnpacking(b,a.sensorId);
    doParsimUnpacking(b,a.hopCount);
}

class SSSensorInfoDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    SSSensorInfoDescriptor();
    virtual ~SSSensorInfoDescriptor();

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

Register_ClassDescriptor(SSSensorInfoDescriptor)

SSSensorInfoDescriptor::SSSensorInfoDescriptor() : omnetpp::cClassDescriptor("SSSensorInfo", "")
{
    propertynames = nullptr;
}

SSSensorInfoDescriptor::~SSSensorInfoDescriptor()
{
    delete[] propertynames;
}

bool SSSensorInfoDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<SSSensorInfo *>(obj)!=nullptr;
}

const char **SSSensorInfoDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *SSSensorInfoDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int SSSensorInfoDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 4+basedesc->getFieldCount() : 4;
}

unsigned int SSSensorInfoDescriptor::getFieldTypeFlags(int field) const
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
    };
    return (field>=0 && field<4) ? fieldTypeFlags[field] : 0;
}

const char *SSSensorInfoDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "destinationCH",
        "dataId",
        "sensorId",
        "hopCount",
    };
    return (field>=0 && field<4) ? fieldNames[field] : nullptr;
}

int SSSensorInfoDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='d' && strcmp(fieldName, "destinationCH")==0) return base+0;
    if (fieldName[0]=='d' && strcmp(fieldName, "dataId")==0) return base+1;
    if (fieldName[0]=='s' && strcmp(fieldName, "sensorId")==0) return base+2;
    if (fieldName[0]=='h' && strcmp(fieldName, "hopCount")==0) return base+3;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *SSSensorInfoDescriptor::getFieldTypeString(int field) const
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
    };
    return (field>=0 && field<4) ? fieldTypeStrings[field] : nullptr;
}

const char **SSSensorInfoDescriptor::getFieldPropertyNames(int field) const
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

const char *SSSensorInfoDescriptor::getFieldProperty(int field, const char *propertyname) const
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

int SSSensorInfoDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    SSSensorInfo *pp = (SSSensorInfo *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

const char *SSSensorInfoDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    SSSensorInfo *pp = (SSSensorInfo *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string SSSensorInfoDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    SSSensorInfo *pp = (SSSensorInfo *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->destinationCH);
        case 1: return long2string(pp->dataId);
        case 2: return long2string(pp->sensorId);
        case 3: return long2string(pp->hopCount);
        default: return "";
    }
}

bool SSSensorInfoDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    SSSensorInfo *pp = (SSSensorInfo *)object; (void)pp;
    switch (field) {
        case 0: pp->destinationCH = string2long(value); return true;
        case 1: pp->dataId = string2long(value); return true;
        case 2: pp->sensorId = string2long(value); return true;
        case 3: pp->hopCount = string2long(value); return true;
        default: return false;
    }
}

const char *SSSensorInfoDescriptor::getFieldStructName(int field) const
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

void *SSSensorInfoDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    SSSensorInfo *pp = (SSSensorInfo *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

Register_Class(SSCellularRoutingPacket)

SSCellularRoutingPacket::SSCellularRoutingPacket(const char *name, short kind) : ::RoutingPacket(name,kind)
{
    this->packetType = 0;
    this->clusterHead = 0;
    this->cellSent = 0;
    this->cellNext = 0;
    this->cellNextNext = 0;
    this->cellSource = 0;
    this->cellDestination = 0;
    this->cellHopCount = 0;
    for (unsigned int i=0; i<1000; i++)
        this->cellPath[i] = 0;
    for (unsigned int i=0; i<1000; i++)
        this->cellPathToDestination[i] = 0;
    this->ttl = 0;
}

SSCellularRoutingPacket::SSCellularRoutingPacket(const SSCellularRoutingPacket& other) : ::RoutingPacket(other)
{
    copy(other);
}

SSCellularRoutingPacket::~SSCellularRoutingPacket()
{
}

SSCellularRoutingPacket& SSCellularRoutingPacket::operator=(const SSCellularRoutingPacket& other)
{
    if (this==&other) return *this;
    ::RoutingPacket::operator=(other);
    copy(other);
    return *this;
}

void SSCellularRoutingPacket::copy(const SSCellularRoutingPacket& other)
{
    this->packetType = other.packetType;
    this->clusterHead = other.clusterHead;
    this->cellSent = other.cellSent;
    this->cellNext = other.cellNext;
    this->cellNextNext = other.cellNextNext;
    this->cellSource = other.cellSource;
    this->cellDestination = other.cellDestination;
    this->cellHopCount = other.cellHopCount;
    for (unsigned int i=0; i<1000; i++)
        this->cellPath[i] = other.cellPath[i];
    for (unsigned int i=0; i<1000; i++)
        this->cellPathToDestination[i] = other.cellPathToDestination[i];
    this->ttl = other.ttl;
    this->sensorData = other.sensorData;
    this->cHAnnouncementData = other.cHAnnouncementData;
    this->sSCellHopAnnouncementData = other.sSCellHopAnnouncementData;
}

void SSCellularRoutingPacket::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::RoutingPacket::parsimPack(b);
    doParsimPacking(b,this->packetType);
    doParsimPacking(b,this->clusterHead);
    doParsimPacking(b,this->cellSent);
    doParsimPacking(b,this->cellNext);
    doParsimPacking(b,this->cellNextNext);
    doParsimPacking(b,this->cellSource);
    doParsimPacking(b,this->cellDestination);
    doParsimPacking(b,this->cellHopCount);
    doParsimArrayPacking(b,this->cellPath,1000);
    doParsimArrayPacking(b,this->cellPathToDestination,1000);
    doParsimPacking(b,this->ttl);
    doParsimPacking(b,this->sensorData);
    doParsimPacking(b,this->cHAnnouncementData);
    doParsimPacking(b,this->sSCellHopAnnouncementData);
}

void SSCellularRoutingPacket::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::RoutingPacket::parsimUnpack(b);
    doParsimUnpacking(b,this->packetType);
    doParsimUnpacking(b,this->clusterHead);
    doParsimUnpacking(b,this->cellSent);
    doParsimUnpacking(b,this->cellNext);
    doParsimUnpacking(b,this->cellNextNext);
    doParsimUnpacking(b,this->cellSource);
    doParsimUnpacking(b,this->cellDestination);
    doParsimUnpacking(b,this->cellHopCount);
    doParsimArrayUnpacking(b,this->cellPath,1000);
    doParsimArrayUnpacking(b,this->cellPathToDestination,1000);
    doParsimUnpacking(b,this->ttl);
    doParsimUnpacking(b,this->sensorData);
    doParsimUnpacking(b,this->cHAnnouncementData);
    doParsimUnpacking(b,this->sSCellHopAnnouncementData);
}

int SSCellularRoutingPacket::getPacketType() const
{
    return this->packetType;
}

void SSCellularRoutingPacket::setPacketType(int packetType)
{
    this->packetType = packetType;
}

int SSCellularRoutingPacket::getClusterHead() const
{
    return this->clusterHead;
}

void SSCellularRoutingPacket::setClusterHead(int clusterHead)
{
    this->clusterHead = clusterHead;
}

int SSCellularRoutingPacket::getCellSent() const
{
    return this->cellSent;
}

void SSCellularRoutingPacket::setCellSent(int cellSent)
{
    this->cellSent = cellSent;
}

int SSCellularRoutingPacket::getCellNext() const
{
    return this->cellNext;
}

void SSCellularRoutingPacket::setCellNext(int cellNext)
{
    this->cellNext = cellNext;
}

int SSCellularRoutingPacket::getCellNextNext() const
{
    return this->cellNextNext;
}

void SSCellularRoutingPacket::setCellNextNext(int cellNextNext)
{
    this->cellNextNext = cellNextNext;
}

int SSCellularRoutingPacket::getCellSource() const
{
    return this->cellSource;
}

void SSCellularRoutingPacket::setCellSource(int cellSource)
{
    this->cellSource = cellSource;
}

int SSCellularRoutingPacket::getCellDestination() const
{
    return this->cellDestination;
}

void SSCellularRoutingPacket::setCellDestination(int cellDestination)
{
    this->cellDestination = cellDestination;
}

int SSCellularRoutingPacket::getCellHopCount() const
{
    return this->cellHopCount;
}

void SSCellularRoutingPacket::setCellHopCount(int cellHopCount)
{
    this->cellHopCount = cellHopCount;
}

unsigned int SSCellularRoutingPacket::getCellPathArraySize() const
{
    return 1000;
}

int SSCellularRoutingPacket::getCellPath(unsigned int k) const
{
    if (k>=1000) throw omnetpp::cRuntimeError("Array of size 1000 indexed by %lu", (unsigned long)k);
    return this->cellPath[k];
}

void SSCellularRoutingPacket::setCellPath(unsigned int k, int cellPath)
{
    if (k>=1000) throw omnetpp::cRuntimeError("Array of size 1000 indexed by %lu", (unsigned long)k);
    this->cellPath[k] = cellPath;
}

unsigned int SSCellularRoutingPacket::getCellPathToDestinationArraySize() const
{
    return 1000;
}

int SSCellularRoutingPacket::getCellPathToDestination(unsigned int k) const
{
    if (k>=1000) throw omnetpp::cRuntimeError("Array of size 1000 indexed by %lu", (unsigned long)k);
    return this->cellPathToDestination[k];
}

void SSCellularRoutingPacket::setCellPathToDestination(unsigned int k, int cellPathToDestination)
{
    if (k>=1000) throw omnetpp::cRuntimeError("Array of size 1000 indexed by %lu", (unsigned long)k);
    this->cellPathToDestination[k] = cellPathToDestination;
}

int SSCellularRoutingPacket::getTtl() const
{
    return this->ttl;
}

void SSCellularRoutingPacket::setTtl(int ttl)
{
    this->ttl = ttl;
}

SSSensorInfo& SSCellularRoutingPacket::getSensorData()
{
    return this->sensorData;
}

void SSCellularRoutingPacket::setSensorData(const SSSensorInfo& sensorData)
{
    this->sensorData = sensorData;
}

SSCHAnnouncementInfo& SSCellularRoutingPacket::getCHAnnouncementData()
{
    return this->cHAnnouncementData;
}

void SSCellularRoutingPacket::setCHAnnouncementData(const SSCHAnnouncementInfo& cHAnnouncementData)
{
    this->cHAnnouncementData = cHAnnouncementData;
}

SSCellHopAnnouncementInfo& SSCellularRoutingPacket::getSSCellHopAnnouncementData()
{
    return this->sSCellHopAnnouncementData;
}

void SSCellularRoutingPacket::setSSCellHopAnnouncementData(const SSCellHopAnnouncementInfo& sSCellHopAnnouncementData)
{
    this->sSCellHopAnnouncementData = sSCellHopAnnouncementData;
}

class SSCellularRoutingPacketDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    SSCellularRoutingPacketDescriptor();
    virtual ~SSCellularRoutingPacketDescriptor();

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

Register_ClassDescriptor(SSCellularRoutingPacketDescriptor)

SSCellularRoutingPacketDescriptor::SSCellularRoutingPacketDescriptor() : omnetpp::cClassDescriptor("SSCellularRoutingPacket", "RoutingPacket")
{
    propertynames = nullptr;
}

SSCellularRoutingPacketDescriptor::~SSCellularRoutingPacketDescriptor()
{
    delete[] propertynames;
}

bool SSCellularRoutingPacketDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<SSCellularRoutingPacket *>(obj)!=nullptr;
}

const char **SSCellularRoutingPacketDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *SSCellularRoutingPacketDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int SSCellularRoutingPacketDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 14+basedesc->getFieldCount() : 14;
}

unsigned int SSCellularRoutingPacketDescriptor::getFieldTypeFlags(int field) const
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
        FD_ISARRAY | FD_ISEDITABLE,
        FD_ISARRAY | FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISCOMPOUND,
        FD_ISCOMPOUND,
        FD_ISCOMPOUND,
    };
    return (field>=0 && field<14) ? fieldTypeFlags[field] : 0;
}

const char *SSCellularRoutingPacketDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "packetType",
        "clusterHead",
        "cellSent",
        "cellNext",
        "cellNextNext",
        "cellSource",
        "cellDestination",
        "cellHopCount",
        "cellPath",
        "cellPathToDestination",
        "ttl",
        "sensorData",
        "cHAnnouncementData",
        "sSCellHopAnnouncementData",
    };
    return (field>=0 && field<14) ? fieldNames[field] : nullptr;
}

int SSCellularRoutingPacketDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='p' && strcmp(fieldName, "packetType")==0) return base+0;
    if (fieldName[0]=='c' && strcmp(fieldName, "clusterHead")==0) return base+1;
    if (fieldName[0]=='c' && strcmp(fieldName, "cellSent")==0) return base+2;
    if (fieldName[0]=='c' && strcmp(fieldName, "cellNext")==0) return base+3;
    if (fieldName[0]=='c' && strcmp(fieldName, "cellNextNext")==0) return base+4;
    if (fieldName[0]=='c' && strcmp(fieldName, "cellSource")==0) return base+5;
    if (fieldName[0]=='c' && strcmp(fieldName, "cellDestination")==0) return base+6;
    if (fieldName[0]=='c' && strcmp(fieldName, "cellHopCount")==0) return base+7;
    if (fieldName[0]=='c' && strcmp(fieldName, "cellPath")==0) return base+8;
    if (fieldName[0]=='c' && strcmp(fieldName, "cellPathToDestination")==0) return base+9;
    if (fieldName[0]=='t' && strcmp(fieldName, "ttl")==0) return base+10;
    if (fieldName[0]=='s' && strcmp(fieldName, "sensorData")==0) return base+11;
    if (fieldName[0]=='c' && strcmp(fieldName, "cHAnnouncementData")==0) return base+12;
    if (fieldName[0]=='s' && strcmp(fieldName, "sSCellHopAnnouncementData")==0) return base+13;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *SSCellularRoutingPacketDescriptor::getFieldTypeString(int field) const
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
        "int",
        "int",
        "int",
        "int",
        "int",
        "int",
        "int",
        "SSSensorInfo",
        "SSCHAnnouncementInfo",
        "SSCellHopAnnouncementInfo",
    };
    return (field>=0 && field<14) ? fieldTypeStrings[field] : nullptr;
}

const char **SSCellularRoutingPacketDescriptor::getFieldPropertyNames(int field) const
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

const char *SSCellularRoutingPacketDescriptor::getFieldProperty(int field, const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldProperty(field, propertyname);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        case 0:
            if (!strcmp(propertyname,"enum")) return "SSCellularRoutingPacketType";
            return nullptr;
        default: return nullptr;
    }
}

int SSCellularRoutingPacketDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    SSCellularRoutingPacket *pp = (SSCellularRoutingPacket *)object; (void)pp;
    switch (field) {
        case 8: return 1000;
        case 9: return 1000;
        default: return 0;
    }
}

const char *SSCellularRoutingPacketDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    SSCellularRoutingPacket *pp = (SSCellularRoutingPacket *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string SSCellularRoutingPacketDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    SSCellularRoutingPacket *pp = (SSCellularRoutingPacket *)object; (void)pp;
    switch (field) {
        case 0: return enum2string(pp->getPacketType(), "SSCellularRoutingPacketType");
        case 1: return long2string(pp->getClusterHead());
        case 2: return long2string(pp->getCellSent());
        case 3: return long2string(pp->getCellNext());
        case 4: return long2string(pp->getCellNextNext());
        case 5: return long2string(pp->getCellSource());
        case 6: return long2string(pp->getCellDestination());
        case 7: return long2string(pp->getCellHopCount());
        case 8: return long2string(pp->getCellPath(i));
        case 9: return long2string(pp->getCellPathToDestination(i));
        case 10: return long2string(pp->getTtl());
        case 11: {std::stringstream out; out << pp->getSensorData(); return out.str();}
        case 12: {std::stringstream out; out << pp->getCHAnnouncementData(); return out.str();}
        case 13: {std::stringstream out; out << pp->getSSCellHopAnnouncementData(); return out.str();}
        default: return "";
    }
}

bool SSCellularRoutingPacketDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    SSCellularRoutingPacket *pp = (SSCellularRoutingPacket *)object; (void)pp;
    switch (field) {
        case 0: pp->setPacketType((SSCellularRoutingPacketType)string2enum(value, "SSCellularRoutingPacketType")); return true;
        case 1: pp->setClusterHead(string2long(value)); return true;
        case 2: pp->setCellSent(string2long(value)); return true;
        case 3: pp->setCellNext(string2long(value)); return true;
        case 4: pp->setCellNextNext(string2long(value)); return true;
        case 5: pp->setCellSource(string2long(value)); return true;
        case 6: pp->setCellDestination(string2long(value)); return true;
        case 7: pp->setCellHopCount(string2long(value)); return true;
        case 8: pp->setCellPath(i,string2long(value)); return true;
        case 9: pp->setCellPathToDestination(i,string2long(value)); return true;
        case 10: pp->setTtl(string2long(value)); return true;
        default: return false;
    }
}

const char *SSCellularRoutingPacketDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        case 11: return omnetpp::opp_typename(typeid(SSSensorInfo));
        case 12: return omnetpp::opp_typename(typeid(SSCHAnnouncementInfo));
        case 13: return omnetpp::opp_typename(typeid(SSCellHopAnnouncementInfo));
        default: return nullptr;
    };
}

void *SSCellularRoutingPacketDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    SSCellularRoutingPacket *pp = (SSCellularRoutingPacket *)object; (void)pp;
    switch (field) {
        case 11: return (void *)(&pp->getSensorData()); break;
        case 12: return (void *)(&pp->getCHAnnouncementData()); break;
        case 13: return (void *)(&pp->getSSCellHopAnnouncementData()); break;
        default: return nullptr;
    }
}


