#include "rapidjson/document.h"
#include "rapidjson/filewritestream.h"
#include <rapidjson/prettywriter.h>

#include <algorithm>
#include <iostream>

using namespace rapidjson;
using namespace std;

static void printIt(const Value &doc) {
    char writeBuffer[65536];
    FileWriteStream os(stdout, writeBuffer, sizeof(writeBuffer));
    PrettyWriter<FileWriteStream> writer(os);
    doc.Accept(writer);
    cout << endl;
}

struct NameComparator {
    bool operator()(
        const GenericMember<UTF8<>, MemoryPoolAllocator<> > &lhs,
        const GenericMember<UTF8<>, MemoryPoolAllocator<> > &rhs) const
    {
        return (strcmp(lhs.name.GetString(), rhs.name.GetString()) < 0);
    }
};

int main() {
    Document d = Document(kObjectType);
    Document::AllocatorType &allocator = d.GetAllocator();

    d.AddMember("zeta", Value().SetBool(false), allocator);
    d.AddMember("gama", Value().SetString("test string", allocator), allocator);
    d.AddMember("delta", Value().SetInt(123), allocator);
    d.AddMember("alpha", Value(kArrayType).Move(), allocator);

    printIt(d);

/*
{
    "zeta": false,
    "gama": "test string",
    "delta": 123,
    "alpha": []
}
*/

    std::sort(d.MemberBegin(), d.MemberEnd(), NameComparator());
    printIt(d);

/*
{
  "alpha": [],
  "delta": 123,
  "gama": "test string",
  "zeta": false
}
*/
}