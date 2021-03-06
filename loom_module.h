/*
 * HUMANOBS - D1
 *
 * Eric Nivel
 * Center for Analysis and Design of Intelligent Agents
 *   Reykjavik University, Menntavegur 1, 101 Reykjavik, Iceland
 *   http://cadia.ru.is
 * Copyright(c)2012
 *
 * This software was developed by the above copyright holder as part of
 * the HUMANOBS EU research project, in collaboration with the
 * following parties:
 *
 * Autonomous Systems Laboratory
 *   Technical University of Madrid, Spain
 *   http://www.aslab.org/
 *
 * Communicative Machines
 *   Edinburgh, United Kingdom
 *   http://www.cmlabs.com/
 *
 * Istituto Dalle Molle di Studi sull'Intelligenza Artificiale
 *   University of Lugano and SUPSI, Switzerland
 *   http://www.idsia.ch/
 *
 * Institute of Cognitive Sciences and Technologies
 *   Consiglio Nazionale delle Ricerche, Italy
 *   http://www.istc.cnr.it/
 *
 * Dipartimento di Ingegneria Informatica
 *   University of Palermo, Italy
 *   http://roboticslab.dinfo.unipa.it/index.php/Main/HomePage
 *
 *
 * --- HUMANOBS Open-Source BSD License, with CADIA Clause v 1.0 ---
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, is permitted provided that the following conditions
 * are met:
 *
 * - Redistributions of source code must retain the above copyright
 * and collaboration notice, this list of conditions and the
 * following disclaimer.
 *
 * - Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following
 * disclaimer in the documentation and/or other materials provided
 * with the distribution.
 *
 * - Neither the name of its copyright holders nor the names of its
 * contributors may be used to endorse or promote products
 * derived from this software without specific prior written permission.
 *
 * - CADIA Clause: The license granted in and to the software under this
 * agreement is a limited-use license. The software may not be used in
 * furtherance of:
 * (i) intentionally causing bodily injury or severe emotional distress
 * to any person;
 * (ii) invading the personal privacy or violating the human rights of
 * any person; or
 * (iii) committing or preparing for any act of war.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef loom_module_h
#define loom_module_h

#include "integration.h"
#include <Core/module_node.h>


#define N module::Node
#define NODE module::Node::Get()
#define OUTPUT NODE->trace(N::APPLICATION)

MODULE_CLASS_BEGIN(Loom_module, Module<Loom_module>)
public:
SharedLibrary *lib;

typedef void (*OutputToDevices)(_Payload *p);
OutputToDevices output_to_devices;

typedef void (*SetModule)(Loom_module *iModule);
SetModule set_module;

std::string device_hub_path;

void loadParameters(const std::vector<int32_t> &numbers, const std::vector<std::string> &strings)
{
    device_hub_path = strings[0];
}

void start()
{
    output_to_devices = NULL;
    lib = SharedLibrary::New(device_hub_path.c_str());

    if (lib) {
        output_to_devices = lib->getFunction<OutputToDevices>("ProcessPayload");
        set_module        = lib->getFunction<SetModule>("SetModule");
    }
}
void stop()
{
    NODE->send(this, new StopMem(), N::PRIMARY);

    if (lib) {
        delete lib;
    }
}
template<class T> Decision decide(T *p)
{
    return WAIT;
}
template<class T> void react(T *p) {}

void react(SystemReady *p)
{
    OUTPUT << "Loom " << "got SysReady" << std::endl;

    if (set_module) {
        set_module(this);
    }

    if (output_to_devices) {
        output_to_devices(p);
    }

    uint16_t vId = StartMem::CID();
    NODE->send(this, new StartMem(), N::PRIMARY);
// for testing.
//Sleep(1000);
//NODE->send(this,new StopMem(),N::PRIMARY);
}

// rMem -> devices -> Loom.
/*
void (*mem_ready)(_Payload *);
void (*set_ontology_count)(_Payload *);
void (*set_ontology_member)(_Payload *);
void (*speak)(_Payload *);
void (*move_hand)(_Payload *);
void (*grab_hand)(_Payload *);
void (*release_hand)(_Payload *);
void (*point_at)(_Payload *);
void (*look_at)(_Payload *);
*/
void react(MemReady *p)
{
    if (output_to_devices) {
        output_to_devices(p);
    }
}
void react(OntologyCount *p)
{
//OUTPUT<<"got ontology count: "<<p->count<<std::endl;
    if (output_to_devices) {
        output_to_devices(p);
    }
}
void react(OntologyDef *p)
{
//OUTPUT<<"got ontology member: "<<p->name<<std::endl;
    if (output_to_devices) {
        output_to_devices(p);
    }
}
void react(Speak *p)
{
    OUTPUT << "RMem says: " << p->word << std::endl;

    if (output_to_devices) {
        output_to_devices(p);
    }
}
void react(MoveTo *p)
{
    if (output_to_devices) {
        output_to_devices(p);
    }
}
void react(PointAt *p)
{
    if (output_to_devices) {
        output_to_devices(p);
    }
}
void react(Grab *p)
{
    if (output_to_devices) {
        output_to_devices(p);
    }
}
void react(Release *p)
{
    if (output_to_devices) {
        output_to_devices(p);
    }
}
void react(LookAt *p)
{
    if (output_to_devices) {
        output_to_devices(p);
    }
}
void react(Sample_Vec3 *p)
{
    if (output_to_devices) {
        output_to_devices(p);
    }
}

void react(Bones *p)
{
    if (output_to_devices) {
        output_to_devices(p);
    }
}

void react(Sample_String255 *sample)
{
    int a = 0;
}

void on_heard(const char *iText)
{
    Sample_String255 *vText = new Sample_String255;
    memset(vText->value, 0, 255);
    strncpy(vText->value, iText, 254);
    NODE->send(this, vText, N::PRIMARY);
}

MODULE_CLASS_END(Loom_module)

#endif
