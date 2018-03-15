#include "namegenerator.hpp"
#include <sstream>

namespace
{

/*
 * Word list created by Hay Kranen
 * https://gist.github.com/hay/1351230
 */
static const std::vector<std::string> Words
{{
        "Factory", "Bean", "Wrapper", "Visitor", "Model", "Singleton",
        "Method", "Configuration", "Exception", "Error", "Property", "Value",
        "Identifier", "Attribute", "Authentication", "Policy", "Container",
        "Order", "Info", "Parameter", "Request", "Adapter", "Bridge",
        "Decorator", "Facade", "Proxy", "Worker",
        "Interpreter", "Iterator", "Observer",
        "State", "Strategy", "Template", "Comparator", "Clone", "Task",
        "Resolver", "Candidate", "Expression", "Predicate",
        "Thread", "Pool", "Descriptor", "Interceptor", "Definition",
        "Getter", "Setter", "Listener", "Proccesor", "Printer",
        "Prototype", "Composer", "Event", "Helper", "Utils",
        "Invocation", "Exporter", "Importer", "Serializer", "Callback",
        "Tag", "Context", "Mapping", "Advisor", "Filter", "Field", "Test",
        "Tests", "Connection", "Annotation", "Service", "Repository",
        "Stub", "Mock", "Instance", "Dispatcher", "Client", "Server",
        "Message", "Map", "List", "Collection", "Queue", "Manager",
        "Database", "Reponse", "Broadcaster",
        "Watcher", "Schema", "Mapper", "Publisher", "Consumer", "Producer",
        "Composite", "Invalid", "Supported", "Focus", "Traversal", "Abstract",
        "Transformer", "Common", "Concrete", "Autowire", "Simple", "Aware",
        "Aspect", "Principal", "Driven", "Interruptible", "Batch",
        "Prepared", "Statement", "Remote", "Stateless", "Session",
        "Transaction", "Transactional", "Based", "Meta", "Data", "Jms",
        "Readable", "Literal", "Reflective", "Scope", "Multipart", "Xml",
        "Generic", "Interface", "Advisable", "Observable", "Identifiable",
        "Iterable", "Distributed", "Notification", "Failure", "Type",
        "Http", "Jdbc"
    }};
}

namespace fake
{

NameGenerator::NameGenerator()
    : NameGenerator(std::random_device()())
{}

NameGenerator::NameGenerator(unsigned int seed)
    : m_twister(seed)
    , m_num_words_dist(1, 3)
    , m_words_dist(0, Words.size()-1)
{}

std::string NameGenerator::get_name()
{
    std::stringstream name;
    for(vector_size_t i = 0; i < m_num_words_dist(m_twister); i++)
    {
        // assert index not out of range?
        name << Words.at(m_words_dist(m_twister));
    }

    return name.str();
}

}
