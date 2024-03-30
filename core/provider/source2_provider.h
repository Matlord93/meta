#ifndef _INCLUDE_METAMOD_SOURCE_BASE_PROVIDER_H_
#define _INCLUDE_METAMOD_SOURCE_BASE_PROVIDER_H_

#include <sourcehook.h>
#include <sh_memfuncinfo.h>
#include <eiface.h>
#include <icvar.h>
#include <tier1/utlvector.h>
#include "metamod_provider.h"
#include "metamod_oslink.h"
#include "source2_provider.h"

IVEngineServer* engine = nullptr;
IServerGameDLL* server = nullptr;
IServerGameClients* gameclients = nullptr;
ICvar* icvar = nullptr;
CGlobalVars* gpGlobals = nullptr;
#ifdef META_IS_SOURCE2
INetworkServerService* netservice = nullptr;
IEngineServiceMgr* enginesvcmgr = nullptr;
#endif

class BaseProvider {
public:
    bool IsSourceEngineBuildCompatible(int build);
    bool LogMessage(const char* buffer);
    void DisplayError(const char* fmt, ...);
    void DisplayWarning(const char* fmt, ...);
    void DisplayDevMsg(const char* fmt, ...);
    int TryServerGameDLL(const char* iface);

    virtual void Notify_DLLInit_Pre(CreateInterfaceFn engineFactory, CreateInterfaceFn serverFactory);
    virtual void Notify_DLLShutdown_Pre();
    virtual int DetermineSourceEngine();
    virtual const char* GetEngineDescription() const;
    virtual void GetGamePath(char* pszBuffer, int len);
    virtual const char* GetGameDescription();
    virtual bool ProcessVDF(const char* file, char path[], size_t path_len, char alias[], size_t alias_len);
    virtual const char* GetCommandLineValue(const char* key, const char* defval);
    virtual void ConsolePrint(const char* msg);
    virtual void ClientConsolePrint(MMSPlayer_t player, const char* msg);
    virtual bool RegisterConCommandBase(ConCommandBase* pCommand);
    virtual void UnregisterConCommandBase(ConCommandBase* pCommand);
    virtual bool IsConCommandBaseACommand(ConCommandBase* pCommand);
};

class Source2Provider : public BaseProvider {
public:
    virtual void Notify_DLLInit_Pre(CreateInterfaceFn engineFactory, CreateInterfaceFn serverFactory) override;
    virtual void Notify_DLLShutdown_Pre() override;
    virtual int DetermineSourceEngine() override;
    virtual const char* GetEngineDescription() const override;
    virtual void GetGamePath(char* pszBuffer, int len) override;
    virtual const char* GetGameDescription() override;
    virtual bool ProcessVDF(const char* file, char path[], size_t path_len, char alias[], size_t alias_len) override;
    virtual const char* GetCommandLineValue(const char* key, const char* defval) override;
    virtual void ConsolePrint(const char* msg) override;
    virtual void ClientConsolePrint(MMSPlayer_t player, const char* msg) override;
    virtual bool RegisterConCommandBase(ConCommandBase* pCommand) override;
    virtual void UnregisterConCommandBase(ConCommandBase* pCommand) override;
    virtual bool IsConCommandBaseACommand(ConCommandBase* pCommand) override;

    // Add other overridden methods here...

    // Add other methods specific to Source2Provider here...

bool BaseProvider::IsSourceEngineBuildCompatible(int build)
{
    return SOURCE_ENGINE_BUILD == build;
}

bool BaseProvider::LogMessage(const char *buffer)
{
    Log(LOG_CONSOLE, buffer);
    return true;
}

void BaseProvider::DisplayError(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    char msg[1024];
    vsnprintf(msg, sizeof(msg), fmt, ap);
    va_end(ap);
    Log(LOG_ERROR, msg);
}

void BaseProvider::DisplayWarning(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    char msg[1024];
    vsnprintf(msg, sizeof(msg), fmt, ap);
    va_end(ap);
    Log(LOG_WARNING, msg);
}

void BaseProvider::DisplayDevMsg(const char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    char msg[1024];
    vsnprintf(msg, sizeof(msg), fmt, ap);
    va_end(ap);
    Log(LOG_DEBUG, msg);
}

int BaseProvider::TryServerGameDLL(const char *iface)
{
    return SourceHook::TryServerGameDLL(iface);
}

void Source2Provider::Notify_DLLInit_Pre(CreateInterfaceFn engineFactory, CreateInterfaceFn serverFactory)
{
    engine = (IVEngineServer*)((engineFactory)(INTERFACEVERSION_VENGINESERVER, NULL));
    if (!engine)
    {
        DisplayError("Could not find IVEngineServer! Metamod cannot load.");
        return;
    }

    gpGlobals = engine->GetServerGlobals();

    icvar = (ICvar*)((engineFactory)(CVAR_INTERFACE_VERSION, NULL));
    if (!icvar)
    {
        DisplayError("Could not find ICvar! Metamod cannot load.");
        return;
    }

    gameclients = (IServerGameClients*)(serverFactory(INTERFACEVERSION_SERVERGAMECLIENTS, NULL));
    if (!gameclients)
    {
        DisplayError("Could not find IServerGameClients! Metamod cannot load.");
        return;
    }

    // Initialize other interfaces as needed

    // Call base class implementation
    BaseProvider::Notify_DLLInit_Pre(engineFactory, serverFactory);
}

void Source2Provider::Notify_DLLShutdown_Pre()
{
    // Shutdown logic for Source2Provider

    // Call base class implementation
    BaseProvider::Notify_DLLShutdown_Pre();
}

int Source2Provider::DetermineSourceEngine()
{
    return SOURCE_ENGINE_CS2; // Assuming this is Counter-Strike 2
}

const char* Source2Provider::GetEngineDescription() const
{
    return "Counter-Strike 2 (2023)";
}

void Source2Provider::GetGamePath(char* pszBuffer, int len)
{
    // Implement logic to get the game path
}

const char* Source2Provider::GetGameDescription()
{
    // Implement logic to get the game description
    return nullptr;
}

bool Source2Provider::ProcessVDF(const char* file, char path[], size_t path_len, char alias[], size_t alias_len)
{
    // Implement logic to process VDF files
    return false;
}

const char* Source2Provider::GetCommandLineValue(const char* key, const char* defval)
{
    // Implement logic to get command line values
    return nullptr;
}

void Source2Provider::ConsolePrint(const char* msg)
{
    // Implement logic to print messages to console
}

void Source2Provider::ClientConsolePrint(MMSPlayer_t player, const char* msg)
{
    // Implement logic to print messages to a specific client's console
}

void Source2Provider::ServerCommand(const char* cmd)
{
    // Implement logic to execute server commands
}

bool Source2Provider::RegisterConCommandBase(ConCommandBase* pCommand)
{
    // Implement logic to register console commands
    return false;
}

void Source2Provider::UnregisterConCommandBase(ConCommandBase* pCommand)
{
    // Implement logic to unregister console commands
}

bool Source2Provider::IsConCommandBaseACommand(ConCommandBase* pCommand)
{
    // Implement logic to check if a ConCommandBase is a command
    return false;
}

#ifdef SHOULD_OVERRIDE_ALLOWDEDICATED_SERVER
bool Source2Provider::Hook_AllowDedicatedServers(EUniverse universe) const
{
    // Implement logic to allow dedicated servers
    return true;
}
#endif

void Source2Provider::Hook_RegisterLoopMode(const char *pszLoopModeName, ILoopModeFactory *pLoopModeFactory, void **ppGlobalPointer)
{
    // Implement logic for registering loop modes
}

void Source2Provider::Hook_UnregisterLoopMode(const char* pszLoopModeName, ILoopModeFactory* pLoopModeFactory, void** ppGlobalPointer)
{
    // Implement logic for unregistering loop modes
}

ILoopMode *Source2Provider::Hook_CreateLoopModePost()
{
    // Implement logic for creating loop modes post-hook
    return nullptr;
}

void Source2Provider::Hook_DestroyLoopMode(ILoopMode* pLoopMode)
{
    // Implement logic for destroying loop modes
}

bool Source2Provider::Hook_LoopInitPost(KeyValues* pKeyValues, ILoopModePrerequisiteRegistry *pRegistry)
{
    // Implement logic for loop initialization post-hook
    return true;
}

void Source2Provider::Hook_LoopShutdownPost()
{
    // Implement logic for loop shutdown post-hook
}

void Source2Provider::Hook_ClientCommand(CPlayerSlot nSlot, const CCommand& args)
{
    // Implement logic for handling client commands
}
#endif
