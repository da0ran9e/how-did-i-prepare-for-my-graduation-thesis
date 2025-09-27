//
// Generated file, do not edit! Created by nedtool 5.6 from node/communication/routing/CellularRouting/CellularRoutingPacket.msg.
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
#include "CellularRoutingPacket_m.h"

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
    omnetpp::cEnum *e = omnetpp::cEnum::find("CellularRoutingPacketType");
    if (!e) omnetpp::enums.getInstance()->add(e = new omnetpp::cEnum("CellularRoutingPacketType"));
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

HelloInfo::HelloInfo()
{
    this->cellId = 0;
    this->color = 0;
    this->energyLevel = 0;
    this->x = 0;
    this->y = 0;
}

void __doPacking(omnetpp::cCommBuffer *b, const HelloInfo& a)
{
    doParsimPacking(b,a.cellId);
    doParsimPacking(b,a.color);
    doParsimPacking(b,a.energyLevel);
    doParsimPacking(b,a.x);
    doParsimPacking(b,a.y);
}

void __doUnpacking(omnetpp::cCommBuffer *b, HelloInfo& a)
{
    doParsimUnpacking(b,a.cellId);
    doParsimUnpacking(b,a.color);
    doParsimUnpacking(b,a.energyLevel);
    doParsimUnpacking(b,a.x);
    doParsimUnpacking(b,a.y);
}

class HelloInfoDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    HelloInfoDescriptor();
    virtual ~HelloInfoDescriptor();

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

Register_ClassDescriptor(HelloInfoDescriptor)

HelloInfoDescriptor::HelloInfoDescriptor() : omnetpp::cClassDescriptor("HelloInfo", "")
{
    propertynames = nullptr;
}

HelloInfoDescriptor::~HelloInfoDescriptor()
{
    delete[] propertynames;
}

bool HelloInfoDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<HelloInfo *>(obj)!=nullptr;
}

const char **HelloInfoDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *HelloInfoDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int HelloInfoDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 5+basedesc->getFieldCount() : 5;
}

unsigned int HelloInfoDescriptor::getFieldTypeFlags(int field) const
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
    };
    return (field>=0 && field<5) ? fieldTypeFlags[field] : 0;
}

const char *HelloInfoDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "cellId",
        "color",
        "energyLevel",
        "x",
        "y",
    };
    return (field>=0 && field<5) ? fieldNames[field] : nullptr;
}

int HelloInfoDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='c' && strcmp(fieldName, "cellId")==0) return base+0;
    if (fieldName[0]=='c' && strcmp(fieldName, "color")==0) return base+1;
    if (fieldName[0]=='e' && strcmp(fieldName, "energyLevel")==0) return base+2;
    if (fieldName[0]=='x' && strcmp(fieldName, "x")==0) return base+3;
    if (fieldName[0]=='y' && strcmp(fieldName, "y")==0) return base+4;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *HelloInfoDescriptor::getFieldTypeString(int field) const
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
        "double",
        "double",
    };
    return (field>=0 && field<5) ? fieldTypeStrings[field] : nullptr;
}

const char **HelloInfoDescriptor::getFieldPropertyNames(int field) const
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

const char *HelloInfoDescriptor::getFieldProperty(int field, const char *propertyname) const
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

int HelloInfoDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    HelloInfo *pp = (HelloInfo *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

const char *HelloInfoDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    HelloInfo *pp = (HelloInfo *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string HelloInfoDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    HelloInfo *pp = (HelloInfo *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->cellId);
        case 1: return long2string(pp->color);
        case 2: return double2string(pp->energyLevel);
        case 3: return double2string(pp->x);
        case 4: return double2string(pp->y);
        default: return "";
    }
}

bool HelloInfoDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    HelloInfo *pp = (HelloInfo *)object; (void)pp;
    switch (field) {
        case 0: pp->cellId = string2long(value); return true;
        case 1: pp->color = string2long(value); return true;
        case 2: pp->energyLevel = string2double(value); return true;
        case 3: pp->x = string2double(value); return true;
        case 4: pp->y = string2double(value); return true;
        default: return false;
    }
}

const char *HelloInfoDescriptor::getFieldStructName(int field) const
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

void *HelloInfoDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    HelloInfo *pp = (HelloInfo *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

CLAnnouncementInfo::CLAnnouncementInfo()
{
    this->cellId = 0;
    this->color = 0;
    this->fitnessScore = 0;
    this->x = 0;
    this->y = 0;
}

void __doPacking(omnetpp::cCommBuffer *b, const CLAnnouncementInfo& a)
{
    doParsimPacking(b,a.cellId);
    doParsimPacking(b,a.color);
    doParsimPacking(b,a.fitnessScore);
    doParsimPacking(b,a.x);
    doParsimPacking(b,a.y);
}

void __doUnpacking(omnetpp::cCommBuffer *b, CLAnnouncementInfo& a)
{
    doParsimUnpacking(b,a.cellId);
    doParsimUnpacking(b,a.color);
    doParsimUnpacking(b,a.fitnessScore);
    doParsimUnpacking(b,a.x);
    doParsimUnpacking(b,a.y);
}

class CLAnnouncementInfoDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    CLAnnouncementInfoDescriptor();
    virtual ~CLAnnouncementInfoDescriptor();

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

Register_ClassDescriptor(CLAnnouncementInfoDescriptor)

CLAnnouncementInfoDescriptor::CLAnnouncementInfoDescriptor() : omnetpp::cClassDescriptor("CLAnnouncementInfo", "")
{
    propertynames = nullptr;
}

CLAnnouncementInfoDescriptor::~CLAnnouncementInfoDescriptor()
{
    delete[] propertynames;
}

bool CLAnnouncementInfoDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<CLAnnouncementInfo *>(obj)!=nullptr;
}

const char **CLAnnouncementInfoDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *CLAnnouncementInfoDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int CLAnnouncementInfoDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 5+basedesc->getFieldCount() : 5;
}

unsigned int CLAnnouncementInfoDescriptor::getFieldTypeFlags(int field) const
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
    };
    return (field>=0 && field<5) ? fieldTypeFlags[field] : 0;
}

const char *CLAnnouncementInfoDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "cellId",
        "color",
        "fitnessScore",
        "x",
        "y",
    };
    return (field>=0 && field<5) ? fieldNames[field] : nullptr;
}

int CLAnnouncementInfoDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='c' && strcmp(fieldName, "cellId")==0) return base+0;
    if (fieldName[0]=='c' && strcmp(fieldName, "color")==0) return base+1;
    if (fieldName[0]=='f' && strcmp(fieldName, "fitnessScore")==0) return base+2;
    if (fieldName[0]=='x' && strcmp(fieldName, "x")==0) return base+3;
    if (fieldName[0]=='y' && strcmp(fieldName, "y")==0) return base+4;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *CLAnnouncementInfoDescriptor::getFieldTypeString(int field) const
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
        "double",
        "double",
    };
    return (field>=0 && field<5) ? fieldTypeStrings[field] : nullptr;
}

const char **CLAnnouncementInfoDescriptor::getFieldPropertyNames(int field) const
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

const char *CLAnnouncementInfoDescriptor::getFieldProperty(int field, const char *propertyname) const
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

int CLAnnouncementInfoDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    CLAnnouncementInfo *pp = (CLAnnouncementInfo *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

const char *CLAnnouncementInfoDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    CLAnnouncementInfo *pp = (CLAnnouncementInfo *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string CLAnnouncementInfoDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    CLAnnouncementInfo *pp = (CLAnnouncementInfo *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->cellId);
        case 1: return long2string(pp->color);
        case 2: return double2string(pp->fitnessScore);
        case 3: return double2string(pp->x);
        case 4: return double2string(pp->y);
        default: return "";
    }
}

bool CLAnnouncementInfoDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    CLAnnouncementInfo *pp = (CLAnnouncementInfo *)object; (void)pp;
    switch (field) {
        case 0: pp->cellId = string2long(value); return true;
        case 1: pp->color = string2long(value); return true;
        case 2: pp->fitnessScore = string2double(value); return true;
        case 3: pp->x = string2double(value); return true;
        case 4: pp->y = string2double(value); return true;
        default: return false;
    }
}

const char *CLAnnouncementInfoDescriptor::getFieldStructName(int field) const
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

void *CLAnnouncementInfoDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    CLAnnouncementInfo *pp = (CLAnnouncementInfo *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

NeighborInfo::NeighborInfo()
{
    this->nodeId = 0;
    this->x = 0;
    this->y = 0;
    this->cellId = 0;
}

void __doPacking(omnetpp::cCommBuffer *b, const NeighborInfo& a)
{
    doParsimPacking(b,a.nodeId);
    doParsimPacking(b,a.x);
    doParsimPacking(b,a.y);
    doParsimPacking(b,a.cellId);
}

void __doUnpacking(omnetpp::cCommBuffer *b, NeighborInfo& a)
{
    doParsimUnpacking(b,a.nodeId);
    doParsimUnpacking(b,a.x);
    doParsimUnpacking(b,a.y);
    doParsimUnpacking(b,a.cellId);
}

class NeighborInfoDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    NeighborInfoDescriptor();
    virtual ~NeighborInfoDescriptor();

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

Register_ClassDescriptor(NeighborInfoDescriptor)

NeighborInfoDescriptor::NeighborInfoDescriptor() : omnetpp::cClassDescriptor("NeighborInfo", "")
{
    propertynames = nullptr;
}

NeighborInfoDescriptor::~NeighborInfoDescriptor()
{
    delete[] propertynames;
}

bool NeighborInfoDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<NeighborInfo *>(obj)!=nullptr;
}

const char **NeighborInfoDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *NeighborInfoDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int NeighborInfoDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 4+basedesc->getFieldCount() : 4;
}

unsigned int NeighborInfoDescriptor::getFieldTypeFlags(int field) const
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

const char *NeighborInfoDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "nodeId",
        "x",
        "y",
        "cellId",
    };
    return (field>=0 && field<4) ? fieldNames[field] : nullptr;
}

int NeighborInfoDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='n' && strcmp(fieldName, "nodeId")==0) return base+0;
    if (fieldName[0]=='x' && strcmp(fieldName, "x")==0) return base+1;
    if (fieldName[0]=='y' && strcmp(fieldName, "y")==0) return base+2;
    if (fieldName[0]=='c' && strcmp(fieldName, "cellId")==0) return base+3;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *NeighborInfoDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",
        "double",
        "double",
        "int",
    };
    return (field>=0 && field<4) ? fieldTypeStrings[field] : nullptr;
}

const char **NeighborInfoDescriptor::getFieldPropertyNames(int field) const
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

const char *NeighborInfoDescriptor::getFieldProperty(int field, const char *propertyname) const
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

int NeighborInfoDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    NeighborInfo *pp = (NeighborInfo *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

const char *NeighborInfoDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    NeighborInfo *pp = (NeighborInfo *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string NeighborInfoDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    NeighborInfo *pp = (NeighborInfo *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->nodeId);
        case 1: return double2string(pp->x);
        case 2: return double2string(pp->y);
        case 3: return long2string(pp->cellId);
        default: return "";
    }
}

bool NeighborInfoDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    NeighborInfo *pp = (NeighborInfo *)object; (void)pp;
    switch (field) {
        case 0: pp->nodeId = string2long(value); return true;
        case 1: pp->x = string2double(value); return true;
        case 2: pp->y = string2double(value); return true;
        case 3: pp->cellId = string2long(value); return true;
        default: return false;
    }
}

const char *NeighborInfoDescriptor::getFieldStructName(int field) const
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

void *NeighborInfoDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    NeighborInfo *pp = (NeighborInfo *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

NodeInfo::NodeInfo()
{
    this->nodeId = 0;
    this->x = 0;
    this->y = 0;
    this->energyLevel = 0;
}

void __doPacking(omnetpp::cCommBuffer *b, const NodeInfo& a)
{
    doParsimPacking(b,a.nodeId);
    doParsimPacking(b,a.x);
    doParsimPacking(b,a.y);
    doParsimPacking(b,a.energyLevel);
    doParsimArrayPacking(b,a.neighbors,20);
}

void __doUnpacking(omnetpp::cCommBuffer *b, NodeInfo& a)
{
    doParsimUnpacking(b,a.nodeId);
    doParsimUnpacking(b,a.x);
    doParsimUnpacking(b,a.y);
    doParsimUnpacking(b,a.energyLevel);
    doParsimArrayUnpacking(b,a.neighbors,20);
}

class NodeInfoDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    NodeInfoDescriptor();
    virtual ~NodeInfoDescriptor();

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

Register_ClassDescriptor(NodeInfoDescriptor)

NodeInfoDescriptor::NodeInfoDescriptor() : omnetpp::cClassDescriptor("NodeInfo", "")
{
    propertynames = nullptr;
}

NodeInfoDescriptor::~NodeInfoDescriptor()
{
    delete[] propertynames;
}

bool NodeInfoDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<NodeInfo *>(obj)!=nullptr;
}

const char **NodeInfoDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *NodeInfoDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int NodeInfoDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 5+basedesc->getFieldCount() : 5;
}

unsigned int NodeInfoDescriptor::getFieldTypeFlags(int field) const
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
        FD_ISARRAY | FD_ISCOMPOUND,
    };
    return (field>=0 && field<5) ? fieldTypeFlags[field] : 0;
}

const char *NodeInfoDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "nodeId",
        "x",
        "y",
        "energyLevel",
        "neighbors",
    };
    return (field>=0 && field<5) ? fieldNames[field] : nullptr;
}

int NodeInfoDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='n' && strcmp(fieldName, "nodeId")==0) return base+0;
    if (fieldName[0]=='x' && strcmp(fieldName, "x")==0) return base+1;
    if (fieldName[0]=='y' && strcmp(fieldName, "y")==0) return base+2;
    if (fieldName[0]=='e' && strcmp(fieldName, "energyLevel")==0) return base+3;
    if (fieldName[0]=='n' && strcmp(fieldName, "neighbors")==0) return base+4;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *NodeInfoDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",
        "double",
        "double",
        "double",
        "NeighborInfo",
    };
    return (field>=0 && field<5) ? fieldTypeStrings[field] : nullptr;
}

const char **NodeInfoDescriptor::getFieldPropertyNames(int field) const
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

const char *NodeInfoDescriptor::getFieldProperty(int field, const char *propertyname) const
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

int NodeInfoDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    NodeInfo *pp = (NodeInfo *)object; (void)pp;
    switch (field) {
        case 4: return 20;
        default: return 0;
    }
}

const char *NodeInfoDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    NodeInfo *pp = (NodeInfo *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string NodeInfoDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    NodeInfo *pp = (NodeInfo *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->nodeId);
        case 1: return double2string(pp->x);
        case 2: return double2string(pp->y);
        case 3: return double2string(pp->energyLevel);
        case 4: {std::stringstream out; out << pp->neighbors[i]; return out.str();}
        default: return "";
    }
}

bool NodeInfoDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    NodeInfo *pp = (NodeInfo *)object; (void)pp;
    switch (field) {
        case 0: pp->nodeId = string2long(value); return true;
        case 1: pp->x = string2double(value); return true;
        case 2: pp->y = string2double(value); return true;
        case 3: pp->energyLevel = string2double(value); return true;
        default: return false;
    }
}

const char *NodeInfoDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        case 4: return omnetpp::opp_typename(typeid(NeighborInfo));
        default: return nullptr;
    };
}

void *NodeInfoDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    NodeInfo *pp = (NodeInfo *)object; (void)pp;
    switch (field) {
        case 4: return (void *)(&pp->neighbors[i]); break;
        default: return nullptr;
    }
}

LinkRequestInfo::LinkRequestInfo()
{
    this->source_cell_id = 0;
    this->source_cgw_id = 0;
    this->target_ngw_id = 0;
    this->target_cell_id = 0;
    this->final_ch_id = 0;
}

void __doPacking(omnetpp::cCommBuffer *b, const LinkRequestInfo& a)
{
    doParsimPacking(b,a.source_cell_id);
    doParsimPacking(b,a.source_cgw_id);
    doParsimPacking(b,a.target_ngw_id);
    doParsimPacking(b,a.target_cell_id);
    doParsimPacking(b,a.final_ch_id);
}

void __doUnpacking(omnetpp::cCommBuffer *b, LinkRequestInfo& a)
{
    doParsimUnpacking(b,a.source_cell_id);
    doParsimUnpacking(b,a.source_cgw_id);
    doParsimUnpacking(b,a.target_ngw_id);
    doParsimUnpacking(b,a.target_cell_id);
    doParsimUnpacking(b,a.final_ch_id);
}

class LinkRequestInfoDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    LinkRequestInfoDescriptor();
    virtual ~LinkRequestInfoDescriptor();

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

Register_ClassDescriptor(LinkRequestInfoDescriptor)

LinkRequestInfoDescriptor::LinkRequestInfoDescriptor() : omnetpp::cClassDescriptor("LinkRequestInfo", "")
{
    propertynames = nullptr;
}

LinkRequestInfoDescriptor::~LinkRequestInfoDescriptor()
{
    delete[] propertynames;
}

bool LinkRequestInfoDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<LinkRequestInfo *>(obj)!=nullptr;
}

const char **LinkRequestInfoDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *LinkRequestInfoDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int LinkRequestInfoDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 5+basedesc->getFieldCount() : 5;
}

unsigned int LinkRequestInfoDescriptor::getFieldTypeFlags(int field) const
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
    };
    return (field>=0 && field<5) ? fieldTypeFlags[field] : 0;
}

const char *LinkRequestInfoDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "source_cell_id",
        "source_cgw_id",
        "target_ngw_id",
        "target_cell_id",
        "final_ch_id",
    };
    return (field>=0 && field<5) ? fieldNames[field] : nullptr;
}

int LinkRequestInfoDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='s' && strcmp(fieldName, "source_cell_id")==0) return base+0;
    if (fieldName[0]=='s' && strcmp(fieldName, "source_cgw_id")==0) return base+1;
    if (fieldName[0]=='t' && strcmp(fieldName, "target_ngw_id")==0) return base+2;
    if (fieldName[0]=='t' && strcmp(fieldName, "target_cell_id")==0) return base+3;
    if (fieldName[0]=='f' && strcmp(fieldName, "final_ch_id")==0) return base+4;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *LinkRequestInfoDescriptor::getFieldTypeString(int field) const
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
    };
    return (field>=0 && field<5) ? fieldTypeStrings[field] : nullptr;
}

const char **LinkRequestInfoDescriptor::getFieldPropertyNames(int field) const
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

const char *LinkRequestInfoDescriptor::getFieldProperty(int field, const char *propertyname) const
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

int LinkRequestInfoDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    LinkRequestInfo *pp = (LinkRequestInfo *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

const char *LinkRequestInfoDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    LinkRequestInfo *pp = (LinkRequestInfo *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string LinkRequestInfoDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    LinkRequestInfo *pp = (LinkRequestInfo *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->source_cell_id);
        case 1: return long2string(pp->source_cgw_id);
        case 2: return long2string(pp->target_ngw_id);
        case 3: return long2string(pp->target_cell_id);
        case 4: return long2string(pp->final_ch_id);
        default: return "";
    }
}

bool LinkRequestInfoDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    LinkRequestInfo *pp = (LinkRequestInfo *)object; (void)pp;
    switch (field) {
        case 0: pp->source_cell_id = string2long(value); return true;
        case 1: pp->source_cgw_id = string2long(value); return true;
        case 2: pp->target_ngw_id = string2long(value); return true;
        case 3: pp->target_cell_id = string2long(value); return true;
        case 4: pp->final_ch_id = string2long(value); return true;
        default: return false;
    }
}

const char *LinkRequestInfoDescriptor::getFieldStructName(int field) const
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

void *LinkRequestInfoDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    LinkRequestInfo *pp = (LinkRequestInfo *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

CLCommandInfo::CLCommandInfo()
{
    this->source_cgw_id = 0;
    this->target_ngw_id = 0;
    this->target_cell_id = 0;
    this->final_ch_id = 0;
}

void __doPacking(omnetpp::cCommBuffer *b, const CLCommandInfo& a)
{
    doParsimPacking(b,a.source_cgw_id);
    doParsimPacking(b,a.target_ngw_id);
    doParsimPacking(b,a.target_cell_id);
    doParsimPacking(b,a.final_ch_id);
}

void __doUnpacking(omnetpp::cCommBuffer *b, CLCommandInfo& a)
{
    doParsimUnpacking(b,a.source_cgw_id);
    doParsimUnpacking(b,a.target_ngw_id);
    doParsimUnpacking(b,a.target_cell_id);
    doParsimUnpacking(b,a.final_ch_id);
}

class CLCommandInfoDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    CLCommandInfoDescriptor();
    virtual ~CLCommandInfoDescriptor();

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

Register_ClassDescriptor(CLCommandInfoDescriptor)

CLCommandInfoDescriptor::CLCommandInfoDescriptor() : omnetpp::cClassDescriptor("CLCommandInfo", "")
{
    propertynames = nullptr;
}

CLCommandInfoDescriptor::~CLCommandInfoDescriptor()
{
    delete[] propertynames;
}

bool CLCommandInfoDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<CLCommandInfo *>(obj)!=nullptr;
}

const char **CLCommandInfoDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *CLCommandInfoDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int CLCommandInfoDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 4+basedesc->getFieldCount() : 4;
}

unsigned int CLCommandInfoDescriptor::getFieldTypeFlags(int field) const
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

const char *CLCommandInfoDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "source_cgw_id",
        "target_ngw_id",
        "target_cell_id",
        "final_ch_id",
    };
    return (field>=0 && field<4) ? fieldNames[field] : nullptr;
}

int CLCommandInfoDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='s' && strcmp(fieldName, "source_cgw_id")==0) return base+0;
    if (fieldName[0]=='t' && strcmp(fieldName, "target_ngw_id")==0) return base+1;
    if (fieldName[0]=='t' && strcmp(fieldName, "target_cell_id")==0) return base+2;
    if (fieldName[0]=='f' && strcmp(fieldName, "final_ch_id")==0) return base+3;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *CLCommandInfoDescriptor::getFieldTypeString(int field) const
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

const char **CLCommandInfoDescriptor::getFieldPropertyNames(int field) const
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

const char *CLCommandInfoDescriptor::getFieldProperty(int field, const char *propertyname) const
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

int CLCommandInfoDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    CLCommandInfo *pp = (CLCommandInfo *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

const char *CLCommandInfoDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    CLCommandInfo *pp = (CLCommandInfo *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string CLCommandInfoDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    CLCommandInfo *pp = (CLCommandInfo *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->source_cgw_id);
        case 1: return long2string(pp->target_ngw_id);
        case 2: return long2string(pp->target_cell_id);
        case 3: return long2string(pp->final_ch_id);
        default: return "";
    }
}

bool CLCommandInfoDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    CLCommandInfo *pp = (CLCommandInfo *)object; (void)pp;
    switch (field) {
        case 0: pp->source_cgw_id = string2long(value); return true;
        case 1: pp->target_ngw_id = string2long(value); return true;
        case 2: pp->target_cell_id = string2long(value); return true;
        case 3: pp->final_ch_id = string2long(value); return true;
        default: return false;
    }
}

const char *CLCommandInfoDescriptor::getFieldStructName(int field) const
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

void *CLCommandInfoDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    CLCommandInfo *pp = (CLCommandInfo *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

NCLConfirmInfo::NCLConfirmInfo()
{
    this->cgw_id = 0;
    this->ngw_id = 0;
}

void __doPacking(omnetpp::cCommBuffer *b, const NCLConfirmInfo& a)
{
    doParsimPacking(b,a.cgw_id);
    doParsimPacking(b,a.ngw_id);
}

void __doUnpacking(omnetpp::cCommBuffer *b, NCLConfirmInfo& a)
{
    doParsimUnpacking(b,a.cgw_id);
    doParsimUnpacking(b,a.ngw_id);
}

class NCLConfirmInfoDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    NCLConfirmInfoDescriptor();
    virtual ~NCLConfirmInfoDescriptor();

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

Register_ClassDescriptor(NCLConfirmInfoDescriptor)

NCLConfirmInfoDescriptor::NCLConfirmInfoDescriptor() : omnetpp::cClassDescriptor("NCLConfirmInfo", "")
{
    propertynames = nullptr;
}

NCLConfirmInfoDescriptor::~NCLConfirmInfoDescriptor()
{
    delete[] propertynames;
}

bool NCLConfirmInfoDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<NCLConfirmInfo *>(obj)!=nullptr;
}

const char **NCLConfirmInfoDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *NCLConfirmInfoDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int NCLConfirmInfoDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 2+basedesc->getFieldCount() : 2;
}

unsigned int NCLConfirmInfoDescriptor::getFieldTypeFlags(int field) const
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
    };
    return (field>=0 && field<2) ? fieldTypeFlags[field] : 0;
}

const char *NCLConfirmInfoDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "cgw_id",
        "ngw_id",
    };
    return (field>=0 && field<2) ? fieldNames[field] : nullptr;
}

int NCLConfirmInfoDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='c' && strcmp(fieldName, "cgw_id")==0) return base+0;
    if (fieldName[0]=='n' && strcmp(fieldName, "ngw_id")==0) return base+1;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *NCLConfirmInfoDescriptor::getFieldTypeString(int field) const
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

const char **NCLConfirmInfoDescriptor::getFieldPropertyNames(int field) const
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

const char *NCLConfirmInfoDescriptor::getFieldProperty(int field, const char *propertyname) const
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

int NCLConfirmInfoDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    NCLConfirmInfo *pp = (NCLConfirmInfo *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

const char *NCLConfirmInfoDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    NCLConfirmInfo *pp = (NCLConfirmInfo *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string NCLConfirmInfoDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    NCLConfirmInfo *pp = (NCLConfirmInfo *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->cgw_id);
        case 1: return long2string(pp->ngw_id);
        default: return "";
    }
}

bool NCLConfirmInfoDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    NCLConfirmInfo *pp = (NCLConfirmInfo *)object; (void)pp;
    switch (field) {
        case 0: pp->cgw_id = string2long(value); return true;
        case 1: pp->ngw_id = string2long(value); return true;
        default: return false;
    }
}

const char *NCLConfirmInfoDescriptor::getFieldStructName(int field) const
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

void *NCLConfirmInfoDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    NCLConfirmInfo *pp = (NCLConfirmInfo *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

RoutingUpdateInfo::RoutingUpdateInfo()
{
    this->nodeId = 0;
    this->fromCell = 0;
    this->toCell = 0;
    this->nextHop = 0;
}

void __doPacking(omnetpp::cCommBuffer *b, const RoutingUpdateInfo& a)
{
    doParsimPacking(b,a.nodeId);
    doParsimPacking(b,a.fromCell);
    doParsimPacking(b,a.toCell);
    doParsimPacking(b,a.nextHop);
}

void __doUnpacking(omnetpp::cCommBuffer *b, RoutingUpdateInfo& a)
{
    doParsimUnpacking(b,a.nodeId);
    doParsimUnpacking(b,a.fromCell);
    doParsimUnpacking(b,a.toCell);
    doParsimUnpacking(b,a.nextHop);
}

class RoutingUpdateInfoDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    RoutingUpdateInfoDescriptor();
    virtual ~RoutingUpdateInfoDescriptor();

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

Register_ClassDescriptor(RoutingUpdateInfoDescriptor)

RoutingUpdateInfoDescriptor::RoutingUpdateInfoDescriptor() : omnetpp::cClassDescriptor("RoutingUpdateInfo", "")
{
    propertynames = nullptr;
}

RoutingUpdateInfoDescriptor::~RoutingUpdateInfoDescriptor()
{
    delete[] propertynames;
}

bool RoutingUpdateInfoDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<RoutingUpdateInfo *>(obj)!=nullptr;
}

const char **RoutingUpdateInfoDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *RoutingUpdateInfoDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int RoutingUpdateInfoDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 4+basedesc->getFieldCount() : 4;
}

unsigned int RoutingUpdateInfoDescriptor::getFieldTypeFlags(int field) const
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

const char *RoutingUpdateInfoDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "nodeId",
        "fromCell",
        "toCell",
        "nextHop",
    };
    return (field>=0 && field<4) ? fieldNames[field] : nullptr;
}

int RoutingUpdateInfoDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='n' && strcmp(fieldName, "nodeId")==0) return base+0;
    if (fieldName[0]=='f' && strcmp(fieldName, "fromCell")==0) return base+1;
    if (fieldName[0]=='t' && strcmp(fieldName, "toCell")==0) return base+2;
    if (fieldName[0]=='n' && strcmp(fieldName, "nextHop")==0) return base+3;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *RoutingUpdateInfoDescriptor::getFieldTypeString(int field) const
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

const char **RoutingUpdateInfoDescriptor::getFieldPropertyNames(int field) const
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

const char *RoutingUpdateInfoDescriptor::getFieldProperty(int field, const char *propertyname) const
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

int RoutingUpdateInfoDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    RoutingUpdateInfo *pp = (RoutingUpdateInfo *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

const char *RoutingUpdateInfoDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    RoutingUpdateInfo *pp = (RoutingUpdateInfo *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string RoutingUpdateInfoDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    RoutingUpdateInfo *pp = (RoutingUpdateInfo *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->nodeId);
        case 1: return long2string(pp->fromCell);
        case 2: return long2string(pp->toCell);
        case 3: return long2string(pp->nextHop);
        default: return "";
    }
}

bool RoutingUpdateInfoDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    RoutingUpdateInfo *pp = (RoutingUpdateInfo *)object; (void)pp;
    switch (field) {
        case 0: pp->nodeId = string2long(value); return true;
        case 1: pp->fromCell = string2long(value); return true;
        case 2: pp->toCell = string2long(value); return true;
        case 3: pp->nextHop = string2long(value); return true;
        default: return false;
    }
}

const char *RoutingUpdateInfoDescriptor::getFieldStructName(int field) const
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

void *RoutingUpdateInfoDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    RoutingUpdateInfo *pp = (RoutingUpdateInfo *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

CHAnnouncementInfo::CHAnnouncementInfo()
{
    this->chId = 0;
}

void __doPacking(omnetpp::cCommBuffer *b, const CHAnnouncementInfo& a)
{
    doParsimPacking(b,a.chId);
}

void __doUnpacking(omnetpp::cCommBuffer *b, CHAnnouncementInfo& a)
{
    doParsimUnpacking(b,a.chId);
}

class CHAnnouncementInfoDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    CHAnnouncementInfoDescriptor();
    virtual ~CHAnnouncementInfoDescriptor();

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

Register_ClassDescriptor(CHAnnouncementInfoDescriptor)

CHAnnouncementInfoDescriptor::CHAnnouncementInfoDescriptor() : omnetpp::cClassDescriptor("CHAnnouncementInfo", "")
{
    propertynames = nullptr;
}

CHAnnouncementInfoDescriptor::~CHAnnouncementInfoDescriptor()
{
    delete[] propertynames;
}

bool CHAnnouncementInfoDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<CHAnnouncementInfo *>(obj)!=nullptr;
}

const char **CHAnnouncementInfoDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *CHAnnouncementInfoDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int CHAnnouncementInfoDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 1+basedesc->getFieldCount() : 1;
}

unsigned int CHAnnouncementInfoDescriptor::getFieldTypeFlags(int field) const
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

const char *CHAnnouncementInfoDescriptor::getFieldName(int field) const
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

int CHAnnouncementInfoDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='c' && strcmp(fieldName, "chId")==0) return base+0;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *CHAnnouncementInfoDescriptor::getFieldTypeString(int field) const
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

const char **CHAnnouncementInfoDescriptor::getFieldPropertyNames(int field) const
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

const char *CHAnnouncementInfoDescriptor::getFieldProperty(int field, const char *propertyname) const
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

int CHAnnouncementInfoDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    CHAnnouncementInfo *pp = (CHAnnouncementInfo *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

const char *CHAnnouncementInfoDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    CHAnnouncementInfo *pp = (CHAnnouncementInfo *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string CHAnnouncementInfoDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    CHAnnouncementInfo *pp = (CHAnnouncementInfo *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->chId);
        default: return "";
    }
}

bool CHAnnouncementInfoDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    CHAnnouncementInfo *pp = (CHAnnouncementInfo *)object; (void)pp;
    switch (field) {
        case 0: pp->chId = string2long(value); return true;
        default: return false;
    }
}

const char *CHAnnouncementInfoDescriptor::getFieldStructName(int field) const
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

void *CHAnnouncementInfoDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    CHAnnouncementInfo *pp = (CHAnnouncementInfo *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

CellHopAnnouncementInfo::CellHopAnnouncementInfo()
{
    this->nextCell = 0;
    for (unsigned int i=0; i<100; i++)
        this->cellPath[i] = 0;
}

void __doPacking(omnetpp::cCommBuffer *b, const CellHopAnnouncementInfo& a)
{
    doParsimPacking(b,a.nextCell);
    doParsimArrayPacking(b,a.cellPath,100);
}

void __doUnpacking(omnetpp::cCommBuffer *b, CellHopAnnouncementInfo& a)
{
    doParsimUnpacking(b,a.nextCell);
    doParsimArrayUnpacking(b,a.cellPath,100);
}

class CellHopAnnouncementInfoDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    CellHopAnnouncementInfoDescriptor();
    virtual ~CellHopAnnouncementInfoDescriptor();

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

Register_ClassDescriptor(CellHopAnnouncementInfoDescriptor)

CellHopAnnouncementInfoDescriptor::CellHopAnnouncementInfoDescriptor() : omnetpp::cClassDescriptor("CellHopAnnouncementInfo", "")
{
    propertynames = nullptr;
}

CellHopAnnouncementInfoDescriptor::~CellHopAnnouncementInfoDescriptor()
{
    delete[] propertynames;
}

bool CellHopAnnouncementInfoDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<CellHopAnnouncementInfo *>(obj)!=nullptr;
}

const char **CellHopAnnouncementInfoDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *CellHopAnnouncementInfoDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int CellHopAnnouncementInfoDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 2+basedesc->getFieldCount() : 2;
}

unsigned int CellHopAnnouncementInfoDescriptor::getFieldTypeFlags(int field) const
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

const char *CellHopAnnouncementInfoDescriptor::getFieldName(int field) const
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

int CellHopAnnouncementInfoDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='n' && strcmp(fieldName, "nextCell")==0) return base+0;
    if (fieldName[0]=='c' && strcmp(fieldName, "cellPath")==0) return base+1;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *CellHopAnnouncementInfoDescriptor::getFieldTypeString(int field) const
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

const char **CellHopAnnouncementInfoDescriptor::getFieldPropertyNames(int field) const
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

const char *CellHopAnnouncementInfoDescriptor::getFieldProperty(int field, const char *propertyname) const
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

int CellHopAnnouncementInfoDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    CellHopAnnouncementInfo *pp = (CellHopAnnouncementInfo *)object; (void)pp;
    switch (field) {
        case 1: return 100;
        default: return 0;
    }
}

const char *CellHopAnnouncementInfoDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    CellHopAnnouncementInfo *pp = (CellHopAnnouncementInfo *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string CellHopAnnouncementInfoDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    CellHopAnnouncementInfo *pp = (CellHopAnnouncementInfo *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->nextCell);
        case 1: if (i>=100) return "";
                return long2string(pp->cellPath[i]);
        default: return "";
    }
}

bool CellHopAnnouncementInfoDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    CellHopAnnouncementInfo *pp = (CellHopAnnouncementInfo *)object; (void)pp;
    switch (field) {
        case 0: pp->nextCell = string2long(value); return true;
        case 1: if (i>=100) return false;
                pp->cellPath[i] = string2long(value); return true;
        default: return false;
    }
}

const char *CellHopAnnouncementInfoDescriptor::getFieldStructName(int field) const
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

void *CellHopAnnouncementInfoDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    CellHopAnnouncementInfo *pp = (CellHopAnnouncementInfo *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

SensorData::SensorData()
{
    this->destinationCH = 0;
    this->dataId = 0;
    this->sensorId = 0;
    this->hopCount = 0;
}

void __doPacking(omnetpp::cCommBuffer *b, const SensorData& a)
{
    doParsimPacking(b,a.destinationCH);
    doParsimPacking(b,a.dataId);
    doParsimPacking(b,a.sensorId);
    doParsimPacking(b,a.hopCount);
}

void __doUnpacking(omnetpp::cCommBuffer *b, SensorData& a)
{
    doParsimUnpacking(b,a.destinationCH);
    doParsimUnpacking(b,a.dataId);
    doParsimUnpacking(b,a.sensorId);
    doParsimUnpacking(b,a.hopCount);
}

class SensorDataDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    SensorDataDescriptor();
    virtual ~SensorDataDescriptor();

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

Register_ClassDescriptor(SensorDataDescriptor)

SensorDataDescriptor::SensorDataDescriptor() : omnetpp::cClassDescriptor("SensorData", "")
{
    propertynames = nullptr;
}

SensorDataDescriptor::~SensorDataDescriptor()
{
    delete[] propertynames;
}

bool SensorDataDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<SensorData *>(obj)!=nullptr;
}

const char **SensorDataDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *SensorDataDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int SensorDataDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 4+basedesc->getFieldCount() : 4;
}

unsigned int SensorDataDescriptor::getFieldTypeFlags(int field) const
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

const char *SensorDataDescriptor::getFieldName(int field) const
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

int SensorDataDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='d' && strcmp(fieldName, "destinationCH")==0) return base+0;
    if (fieldName[0]=='d' && strcmp(fieldName, "dataId")==0) return base+1;
    if (fieldName[0]=='s' && strcmp(fieldName, "sensorId")==0) return base+2;
    if (fieldName[0]=='h' && strcmp(fieldName, "hopCount")==0) return base+3;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *SensorDataDescriptor::getFieldTypeString(int field) const
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

const char **SensorDataDescriptor::getFieldPropertyNames(int field) const
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

const char *SensorDataDescriptor::getFieldProperty(int field, const char *propertyname) const
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

int SensorDataDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    SensorData *pp = (SensorData *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

const char *SensorDataDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    SensorData *pp = (SensorData *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string SensorDataDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    SensorData *pp = (SensorData *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->destinationCH);
        case 1: return long2string(pp->dataId);
        case 2: return long2string(pp->sensorId);
        case 3: return long2string(pp->hopCount);
        default: return "";
    }
}

bool SensorDataDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    SensorData *pp = (SensorData *)object; (void)pp;
    switch (field) {
        case 0: pp->destinationCH = string2long(value); return true;
        case 1: pp->dataId = string2long(value); return true;
        case 2: pp->sensorId = string2long(value); return true;
        case 3: pp->hopCount = string2long(value); return true;
        default: return false;
    }
}

const char *SensorDataDescriptor::getFieldStructName(int field) const
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

void *SensorDataDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    SensorData *pp = (SensorData *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

Register_Class(CellularRoutingPacket)

CellularRoutingPacket::CellularRoutingPacket(const char *name, short kind) : ::RoutingPacket(name,kind)
{
    this->packetType = 0;
    this->clusterHead = 0;
    this->cellSent = 0;
    this->cellNext = 0;
    this->cellNextNext = 0;
    this->cellSource = 0;
    this->cellDestination = 0;
    this->cellHopCount = 0;
    for (unsigned int i=0; i<100; i++)
        this->cellPath[i] = 0;
    for (unsigned int i=0; i<100; i++)
        this->cellPathToDestination[i] = 0;
    this->ttl = 0;
    this->nextHopId = 0;
}

CellularRoutingPacket::CellularRoutingPacket(const CellularRoutingPacket& other) : ::RoutingPacket(other)
{
    copy(other);
}

CellularRoutingPacket::~CellularRoutingPacket()
{
}

CellularRoutingPacket& CellularRoutingPacket::operator=(const CellularRoutingPacket& other)
{
    if (this==&other) return *this;
    ::RoutingPacket::operator=(other);
    copy(other);
    return *this;
}

void CellularRoutingPacket::copy(const CellularRoutingPacket& other)
{
    this->packetType = other.packetType;
    this->clusterHead = other.clusterHead;
    this->cellSent = other.cellSent;
    this->cellNext = other.cellNext;
    this->cellNextNext = other.cellNextNext;
    this->cellSource = other.cellSource;
    this->cellDestination = other.cellDestination;
    this->cellHopCount = other.cellHopCount;
    for (unsigned int i=0; i<100; i++)
        this->cellPath[i] = other.cellPath[i];
    for (unsigned int i=0; i<100; i++)
        this->cellPathToDestination[i] = other.cellPathToDestination[i];
    this->ttl = other.ttl;
    this->helloData = other.helloData;
    this->clAnnouncementData = other.clAnnouncementData;
    this->nodeInfoData = other.nodeInfoData;
    this->linkRequestData = other.linkRequestData;
    this->clCommandData = other.clCommandData;
    this->nclCommandData = other.nclCommandData;
    for (unsigned int i=0; i<7; i++)
        this->routingUpdateData[i] = other.routingUpdateData[i];
    this->chAnnouncementData = other.chAnnouncementData;
    this->nextHopId = other.nextHopId;
    this->cellHopAnnouncementData = other.cellHopAnnouncementData;
    this->sensorData = other.sensorData;
}

void CellularRoutingPacket::parsimPack(omnetpp::cCommBuffer *b) const
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
    doParsimArrayPacking(b,this->cellPath,100);
    doParsimArrayPacking(b,this->cellPathToDestination,100);
    doParsimPacking(b,this->ttl);
    doParsimPacking(b,this->helloData);
    doParsimPacking(b,this->clAnnouncementData);
    doParsimPacking(b,this->nodeInfoData);
    doParsimPacking(b,this->linkRequestData);
    doParsimPacking(b,this->clCommandData);
    doParsimPacking(b,this->nclCommandData);
    doParsimArrayPacking(b,this->routingUpdateData,7);
    doParsimPacking(b,this->chAnnouncementData);
    doParsimPacking(b,this->nextHopId);
    doParsimPacking(b,this->cellHopAnnouncementData);
    doParsimPacking(b,this->sensorData);
}

void CellularRoutingPacket::parsimUnpack(omnetpp::cCommBuffer *b)
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
    doParsimArrayUnpacking(b,this->cellPath,100);
    doParsimArrayUnpacking(b,this->cellPathToDestination,100);
    doParsimUnpacking(b,this->ttl);
    doParsimUnpacking(b,this->helloData);
    doParsimUnpacking(b,this->clAnnouncementData);
    doParsimUnpacking(b,this->nodeInfoData);
    doParsimUnpacking(b,this->linkRequestData);
    doParsimUnpacking(b,this->clCommandData);
    doParsimUnpacking(b,this->nclCommandData);
    doParsimArrayUnpacking(b,this->routingUpdateData,7);
    doParsimUnpacking(b,this->chAnnouncementData);
    doParsimUnpacking(b,this->nextHopId);
    doParsimUnpacking(b,this->cellHopAnnouncementData);
    doParsimUnpacking(b,this->sensorData);
}

int CellularRoutingPacket::getPacketType() const
{
    return this->packetType;
}

void CellularRoutingPacket::setPacketType(int packetType)
{
    this->packetType = packetType;
}

int CellularRoutingPacket::getClusterHead() const
{
    return this->clusterHead;
}

void CellularRoutingPacket::setClusterHead(int clusterHead)
{
    this->clusterHead = clusterHead;
}

int CellularRoutingPacket::getCellSent() const
{
    return this->cellSent;
}

void CellularRoutingPacket::setCellSent(int cellSent)
{
    this->cellSent = cellSent;
}

int CellularRoutingPacket::getCellNext() const
{
    return this->cellNext;
}

void CellularRoutingPacket::setCellNext(int cellNext)
{
    this->cellNext = cellNext;
}

int CellularRoutingPacket::getCellNextNext() const
{
    return this->cellNextNext;
}

void CellularRoutingPacket::setCellNextNext(int cellNextNext)
{
    this->cellNextNext = cellNextNext;
}

int CellularRoutingPacket::getCellSource() const
{
    return this->cellSource;
}

void CellularRoutingPacket::setCellSource(int cellSource)
{
    this->cellSource = cellSource;
}

int CellularRoutingPacket::getCellDestination() const
{
    return this->cellDestination;
}

void CellularRoutingPacket::setCellDestination(int cellDestination)
{
    this->cellDestination = cellDestination;
}

int CellularRoutingPacket::getCellHopCount() const
{
    return this->cellHopCount;
}

void CellularRoutingPacket::setCellHopCount(int cellHopCount)
{
    this->cellHopCount = cellHopCount;
}

unsigned int CellularRoutingPacket::getCellPathArraySize() const
{
    return 100;
}

int CellularRoutingPacket::getCellPath(unsigned int k) const
{
    if (k>=100) throw omnetpp::cRuntimeError("Array of size 100 indexed by %lu", (unsigned long)k);
    return this->cellPath[k];
}

void CellularRoutingPacket::setCellPath(unsigned int k, int cellPath)
{
    if (k>=100) throw omnetpp::cRuntimeError("Array of size 100 indexed by %lu", (unsigned long)k);
    this->cellPath[k] = cellPath;
}

unsigned int CellularRoutingPacket::getCellPathToDestinationArraySize() const
{
    return 100;
}

int CellularRoutingPacket::getCellPathToDestination(unsigned int k) const
{
    if (k>=100) throw omnetpp::cRuntimeError("Array of size 100 indexed by %lu", (unsigned long)k);
    return this->cellPathToDestination[k];
}

void CellularRoutingPacket::setCellPathToDestination(unsigned int k, int cellPathToDestination)
{
    if (k>=100) throw omnetpp::cRuntimeError("Array of size 100 indexed by %lu", (unsigned long)k);
    this->cellPathToDestination[k] = cellPathToDestination;
}

int CellularRoutingPacket::getTtl() const
{
    return this->ttl;
}

void CellularRoutingPacket::setTtl(int ttl)
{
    this->ttl = ttl;
}

HelloInfo& CellularRoutingPacket::getHelloData()
{
    return this->helloData;
}

void CellularRoutingPacket::setHelloData(const HelloInfo& helloData)
{
    this->helloData = helloData;
}

CLAnnouncementInfo& CellularRoutingPacket::getClAnnouncementData()
{
    return this->clAnnouncementData;
}

void CellularRoutingPacket::setClAnnouncementData(const CLAnnouncementInfo& clAnnouncementData)
{
    this->clAnnouncementData = clAnnouncementData;
}

NodeInfo& CellularRoutingPacket::getNodeInfoData()
{
    return this->nodeInfoData;
}

void CellularRoutingPacket::setNodeInfoData(const NodeInfo& nodeInfoData)
{
    this->nodeInfoData = nodeInfoData;
}

LinkRequestInfo& CellularRoutingPacket::getLinkRequestData()
{
    return this->linkRequestData;
}

void CellularRoutingPacket::setLinkRequestData(const LinkRequestInfo& linkRequestData)
{
    this->linkRequestData = linkRequestData;
}

CLCommandInfo& CellularRoutingPacket::getClCommandData()
{
    return this->clCommandData;
}

void CellularRoutingPacket::setClCommandData(const CLCommandInfo& clCommandData)
{
    this->clCommandData = clCommandData;
}

NCLConfirmInfo& CellularRoutingPacket::getNclCommandData()
{
    return this->nclCommandData;
}

void CellularRoutingPacket::setNclCommandData(const NCLConfirmInfo& nclCommandData)
{
    this->nclCommandData = nclCommandData;
}

unsigned int CellularRoutingPacket::getRoutingUpdateDataArraySize() const
{
    return 7;
}

RoutingUpdateInfo& CellularRoutingPacket::getRoutingUpdateData(unsigned int k)
{
    if (k>=7) throw omnetpp::cRuntimeError("Array of size 7 indexed by %lu", (unsigned long)k);
    return this->routingUpdateData[k];
}

void CellularRoutingPacket::setRoutingUpdateData(unsigned int k, const RoutingUpdateInfo& routingUpdateData)
{
    if (k>=7) throw omnetpp::cRuntimeError("Array of size 7 indexed by %lu", (unsigned long)k);
    this->routingUpdateData[k] = routingUpdateData;
}

CHAnnouncementInfo& CellularRoutingPacket::getChAnnouncementData()
{
    return this->chAnnouncementData;
}

void CellularRoutingPacket::setChAnnouncementData(const CHAnnouncementInfo& chAnnouncementData)
{
    this->chAnnouncementData = chAnnouncementData;
}

int CellularRoutingPacket::getNextHopId() const
{
    return this->nextHopId;
}

void CellularRoutingPacket::setNextHopId(int nextHopId)
{
    this->nextHopId = nextHopId;
}

CellHopAnnouncementInfo& CellularRoutingPacket::getCellHopAnnouncementData()
{
    return this->cellHopAnnouncementData;
}

void CellularRoutingPacket::setCellHopAnnouncementData(const CellHopAnnouncementInfo& cellHopAnnouncementData)
{
    this->cellHopAnnouncementData = cellHopAnnouncementData;
}

SensorData& CellularRoutingPacket::getSensorData()
{
    return this->sensorData;
}

void CellularRoutingPacket::setSensorData(const SensorData& sensorData)
{
    this->sensorData = sensorData;
}

class CellularRoutingPacketDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    CellularRoutingPacketDescriptor();
    virtual ~CellularRoutingPacketDescriptor();

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

Register_ClassDescriptor(CellularRoutingPacketDescriptor)

CellularRoutingPacketDescriptor::CellularRoutingPacketDescriptor() : omnetpp::cClassDescriptor("CellularRoutingPacket", "RoutingPacket")
{
    propertynames = nullptr;
}

CellularRoutingPacketDescriptor::~CellularRoutingPacketDescriptor()
{
    delete[] propertynames;
}

bool CellularRoutingPacketDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<CellularRoutingPacket *>(obj)!=nullptr;
}

const char **CellularRoutingPacketDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *CellularRoutingPacketDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int CellularRoutingPacketDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 22+basedesc->getFieldCount() : 22;
}

unsigned int CellularRoutingPacketDescriptor::getFieldTypeFlags(int field) const
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
        FD_ISCOMPOUND,
        FD_ISCOMPOUND,
        FD_ISCOMPOUND,
        FD_ISARRAY | FD_ISCOMPOUND,
        FD_ISCOMPOUND,
        FD_ISEDITABLE,
        FD_ISCOMPOUND,
        FD_ISCOMPOUND,
    };
    return (field>=0 && field<22) ? fieldTypeFlags[field] : 0;
}

const char *CellularRoutingPacketDescriptor::getFieldName(int field) const
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
        "helloData",
        "clAnnouncementData",
        "nodeInfoData",
        "linkRequestData",
        "clCommandData",
        "nclCommandData",
        "routingUpdateData",
        "chAnnouncementData",
        "nextHopId",
        "cellHopAnnouncementData",
        "sensorData",
    };
    return (field>=0 && field<22) ? fieldNames[field] : nullptr;
}

int CellularRoutingPacketDescriptor::findField(const char *fieldName) const
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
    if (fieldName[0]=='h' && strcmp(fieldName, "helloData")==0) return base+11;
    if (fieldName[0]=='c' && strcmp(fieldName, "clAnnouncementData")==0) return base+12;
    if (fieldName[0]=='n' && strcmp(fieldName, "nodeInfoData")==0) return base+13;
    if (fieldName[0]=='l' && strcmp(fieldName, "linkRequestData")==0) return base+14;
    if (fieldName[0]=='c' && strcmp(fieldName, "clCommandData")==0) return base+15;
    if (fieldName[0]=='n' && strcmp(fieldName, "nclCommandData")==0) return base+16;
    if (fieldName[0]=='r' && strcmp(fieldName, "routingUpdateData")==0) return base+17;
    if (fieldName[0]=='c' && strcmp(fieldName, "chAnnouncementData")==0) return base+18;
    if (fieldName[0]=='n' && strcmp(fieldName, "nextHopId")==0) return base+19;
    if (fieldName[0]=='c' && strcmp(fieldName, "cellHopAnnouncementData")==0) return base+20;
    if (fieldName[0]=='s' && strcmp(fieldName, "sensorData")==0) return base+21;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *CellularRoutingPacketDescriptor::getFieldTypeString(int field) const
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
        "HelloInfo",
        "CLAnnouncementInfo",
        "NodeInfo",
        "LinkRequestInfo",
        "CLCommandInfo",
        "NCLConfirmInfo",
        "RoutingUpdateInfo",
        "CHAnnouncementInfo",
        "int",
        "CellHopAnnouncementInfo",
        "SensorData",
    };
    return (field>=0 && field<22) ? fieldTypeStrings[field] : nullptr;
}

const char **CellularRoutingPacketDescriptor::getFieldPropertyNames(int field) const
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

const char *CellularRoutingPacketDescriptor::getFieldProperty(int field, const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldProperty(field, propertyname);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        case 0:
            if (!strcmp(propertyname,"enum")) return "CellularRoutingPacketType";
            return nullptr;
        default: return nullptr;
    }
}

int CellularRoutingPacketDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    CellularRoutingPacket *pp = (CellularRoutingPacket *)object; (void)pp;
    switch (field) {
        case 8: return 100;
        case 9: return 100;
        case 17: return 7;
        default: return 0;
    }
}

const char *CellularRoutingPacketDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    CellularRoutingPacket *pp = (CellularRoutingPacket *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string CellularRoutingPacketDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    CellularRoutingPacket *pp = (CellularRoutingPacket *)object; (void)pp;
    switch (field) {
        case 0: return enum2string(pp->getPacketType(), "CellularRoutingPacketType");
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
        case 11: {std::stringstream out; out << pp->getHelloData(); return out.str();}
        case 12: {std::stringstream out; out << pp->getClAnnouncementData(); return out.str();}
        case 13: {std::stringstream out; out << pp->getNodeInfoData(); return out.str();}
        case 14: {std::stringstream out; out << pp->getLinkRequestData(); return out.str();}
        case 15: {std::stringstream out; out << pp->getClCommandData(); return out.str();}
        case 16: {std::stringstream out; out << pp->getNclCommandData(); return out.str();}
        case 17: {std::stringstream out; out << pp->getRoutingUpdateData(i); return out.str();}
        case 18: {std::stringstream out; out << pp->getChAnnouncementData(); return out.str();}
        case 19: return long2string(pp->getNextHopId());
        case 20: {std::stringstream out; out << pp->getCellHopAnnouncementData(); return out.str();}
        case 21: {std::stringstream out; out << pp->getSensorData(); return out.str();}
        default: return "";
    }
}

bool CellularRoutingPacketDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    CellularRoutingPacket *pp = (CellularRoutingPacket *)object; (void)pp;
    switch (field) {
        case 0: pp->setPacketType((CellularRoutingPacketType)string2enum(value, "CellularRoutingPacketType")); return true;
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
        case 19: pp->setNextHopId(string2long(value)); return true;
        default: return false;
    }
}

const char *CellularRoutingPacketDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        case 11: return omnetpp::opp_typename(typeid(HelloInfo));
        case 12: return omnetpp::opp_typename(typeid(CLAnnouncementInfo));
        case 13: return omnetpp::opp_typename(typeid(NodeInfo));
        case 14: return omnetpp::opp_typename(typeid(LinkRequestInfo));
        case 15: return omnetpp::opp_typename(typeid(CLCommandInfo));
        case 16: return omnetpp::opp_typename(typeid(NCLConfirmInfo));
        case 17: return omnetpp::opp_typename(typeid(RoutingUpdateInfo));
        case 18: return omnetpp::opp_typename(typeid(CHAnnouncementInfo));
        case 20: return omnetpp::opp_typename(typeid(CellHopAnnouncementInfo));
        case 21: return omnetpp::opp_typename(typeid(SensorData));
        default: return nullptr;
    };
}

void *CellularRoutingPacketDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    CellularRoutingPacket *pp = (CellularRoutingPacket *)object; (void)pp;
    switch (field) {
        case 11: return (void *)(&pp->getHelloData()); break;
        case 12: return (void *)(&pp->getClAnnouncementData()); break;
        case 13: return (void *)(&pp->getNodeInfoData()); break;
        case 14: return (void *)(&pp->getLinkRequestData()); break;
        case 15: return (void *)(&pp->getClCommandData()); break;
        case 16: return (void *)(&pp->getNclCommandData()); break;
        case 17: return (void *)(&pp->getRoutingUpdateData(i)); break;
        case 18: return (void *)(&pp->getChAnnouncementData()); break;
        case 20: return (void *)(&pp->getCellHopAnnouncementData()); break;
        case 21: return (void *)(&pp->getSensorData()); break;
        default: return nullptr;
    }
}


