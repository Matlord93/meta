#ifndef CONCOMMANDBASE_H
#define CONCOMMANDBASE_H

#include <string> // Für den Datentyp std::string

// Definition der Dummy-Klasse ConCommandBase
class ConCommandBase {
public:
    ConCommandBase(const std::string& name) : name(name) {}
    virtual ~ConCommandBase() {}

    virtual void Execute() = 0;
    std::string GetName() const { return name; }

protected:
    std::string name;
};

#endif // CONCOMMANDBASE_H
