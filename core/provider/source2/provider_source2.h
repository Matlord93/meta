/**
 * vim: set ts=4 :
 * ======================================================
 * Metamod:Source
 * Copyright (C) 2004-2023 AlliedModders LLC and authors.
 * All rights reserved.
 * ======================================================
 *
 * This software is provided 'as-is', without any express or implied warranty.
 * In no event will the authors be held liable for any damages arising from
 * the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 * claim that you wrote the original software. If you use this software in a
 * product, an acknowledgment in the product documentation would be
 * appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 * misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 * Version: $Id$
 */

#ifndef PROVIDER_SOURCE2_H
#define PROVIDER_SOURCE2_H

#include "../provider_base.h"
#include <tier1/utlvector.h>
#include <IEngineService.h>
#include <string>
#include <icvar.h>
#include <eiface.h>
#include <iserver.h>
#include "metamod_convar.h" // Add include for MetamodSourceConVar

// Falls notwendig
#if SOURCE_ENGINE == SE_DOTA && defined(_WIN32)
#define SHOULD_OVERRIDE_ALLOWDEDICATED_SERVER
#endif

class INetworkGameClient;
class ISource2WorldSession;

CON_COMMAND_EXTERN(meta, LocalCommand_Meta, "Metamod:Source control options");

static ISource2ServerConfig* serverconfig = nullptr;
INetworkServerService* netservice = nullptr;
IEngineServiceMgr* enginesvcmgr = nullptr;

SH_DECL_HOOK2_void(IServerGameClients, ClientCommand, SH_NOATTRIB, 0, CPlayerSlot, const CCommand&);
SH_DECL_HOOK3_void(IEngineServiceMgr, RegisterLoopMode, SH_NOATTRIB, 0, const char*, ILoopModeFactory*, void**);
SH_DECL_HOOK3_void(IEngineServiceMgr, UnregisterLoopMode, SH_NOATTRIB, 0, const char*, ILoopModeFactory*, void**);
SH_DECL_HOOK0(ILoopModeFactory, CreateLoopMode, SH_NOATTRIB, 0, ILoopMode*);
SH_DECL_HOOK1_void(ILoopModeFactory, DestroyLoopMode, SH_NOATTRIB, 0, ILoopMode*);
SH_DECL_HOOK2(ILoopMode, LoopInit, SH_NOATTRIB, 0, bool, KeyValues*, ILoopModePrerequisiteRegistry*);
SH_DECL_HOOK0_void(ILoopMode, LoopShutdown, SH_NOATTRIB, 0);

#ifdef SHOULD_OVERRIDE_ALLOWDEDICATED_SERVER
SH_DECL_HOOK1(ISource2ServerConfig, AllowDedicatedServers, const, 0, bool, EUniverse);
#endif

class Source2Provider : public BaseProvider {
public:
    Source2Provider() {}
    virtual ~Source2Provider() {}

    // New functions

    virtual void Notify_DLLInit_Pre(CreateInterfaceFn engineFactory, CreateInterfaceFn serverFactory) override;
    virtual void Notify_DLLShutdown_Pre() override;
    virtual bool ProcessVDF(const char* file, char path[], size_t path_len, char alias[], size_t alias_len) override;
    virtual int DetermineSourceEngine() override;
    virtual const char* GetEngineDescription() const override;
    virtual void GetGamePath(char* pszBuffer, int len) override;
    virtual const char* GetGameDescription() override;
    virtual const char* GetCommandLineValue(const char* key, const char* defval) override;
    virtual void ConsolePrint(const char* msg) override;
    virtual void ClientConsolePrint(MMSPlayer_t player, const char* msg) override;
    virtual void ServerCommand(const char* cmd) override;
    virtual bool RegisterConCommandBase(ConCommandBase* pCommand) override;
    virtual void UnregisterConCommandBase(ConCommandBase* pCommand) override;
    virtual bool IsConCommandBaseACommand(ConCommandBase* pCommand) override;

#ifdef SHOULD_OVERRIDE_ALLOWDEDICATED_SERVER
    bool Hook_AllowDedicatedServers(EUniverse universe) const;
#endif
    void Hook_RegisterLoopMode(const char* pszLoopModeName, ILoopModeFactory *pLoopModeFactory, void **ppGlobalPointer);
    void Hook_UnregisterLoopMode(const char* pszLoopModeName, ILoopModeFactory* pLoopModeFactory, void** ppGlobalPointer);
    ILoopMode *Hook_CreateLoopModePost();
    void Hook_DestroyLoopMode(ILoopMode*);
    bool Hook_LoopInitPost(KeyValues* pKeyValues, ILoopModePrerequisiteRegistry *pRegistry);
    void Hook_LoopShutdownPost();
    void Hook_ClientCommand(CPlayerSlot nSlot, const CCommand& args);

private:
    IFileSystem* baseFs = nullptr;

    friend void LocalCommand_Meta(const CCommandContext& context, const CCommand& args);
};

// Global variable
extern Source2Provider g_Source2Provider;

#endif // PROVIDER_SOURCE2_H