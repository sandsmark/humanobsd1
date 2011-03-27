//	d_mem.cpp
//
//	Author: Eric Nivel
//
//	BSD license:
//	Copyright (c) 2010, Eric Nivel
//	All rights reserved.
//	Redistribution and use in source and binary forms, with or without
//	modification, are permitted provided that the following conditions are met:
//
//   - Redistributions of source code must retain the above copyright
//     notice, this list of conditions and the following disclaimer.
//   - Redistributions in binary form must reproduce the above copyright
//     notice, this list of conditions and the following disclaimer in the
//     documentation and/or other materials provided with the distribution.
//   - Neither the name of Eric Nivel nor the
//     names of their contributors may be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
//	THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND ANY
//	EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
//	WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
//	DISCLAIMED. IN NO EVENT SHALL THE REGENTS AND CONTRIBUTORS BE LIABLE FOR ANY
//	DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
//	(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
//	LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
//	ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
//	(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
//	SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include	"d_mem.h"
#include	"r_mem_class.h"


#define	N		module::Node
#define	NODE	module::Node::Get()
#define	OUTPUT	NODE->trace(N::APPLICATION)

DMem::DMem(mBrane::sdk::module::_Module	*m):r_exec::Mem<r_exec::LObject>(),module(m){
}

void	DMem::eject(View	*view,uint16	nodeID){
}

void	DMem::eject(Code	*command,uint16	nodeID){
	
	if(command->code(0).asOpcode()==r_exec::GetOpcode("speak")){

		Speak	*speak=new	Speak();
		std::string	w=r_code::Utils::GetString<Code>(command,1);
		memcpy(speak->word,w.c_str(),w.length());
		NODE->send(module,speak,N::PRIMARY);
	}

	//	Add more command procesing here.
}

void	DMem::inject(Code	*object,uint8	nodeID){

	//object->trace();

	//	Build a default view.
	r_exec::View	*view=new	r_exec::View();
	const	uint32	arity=VIEW_ARITY;	//	reminder: opcode not included in the arity.
	uint16	write_index=0;
	uint16	extent_index=arity+1;

	view->code(VIEW_OPCODE)=Atom::SSet(r_exec::View::ViewOpcode,arity);
	view->code(VIEW_SYNC)=Atom::Boolean(false);				//	sync on state.
	view->code(VIEW_IJT)=Atom::IPointer(extent_index);		//	iptr to injection time.
	view->code(VIEW_SLN)=Atom::Float(1.0);					//	sln.
	view->code(VIEW_RES)=Atom::Float(1.0);					//	res is set to 1 upr of the destination group.
	view->code(VIEW_HOST)=Atom::RPointer(0);				//	stdin/stdout is the only reference.
	view->code(VIEW_ORG)=Atom::Node(nodeID);				//	org.

	Utils::SetTimestamp(&view->code(extent_index),r_exec::Now());

	view->references[0]=get_stdin();

	//	Inject the view.
	Code	*_object=check_existence(object);
	view->set_object(_object);
	((_Mem	*)this)->inject(view);
}

Code	*DMem::get_object(uint32	OID,uint8	NID){

	UNORDERED_MAP<uint32,P<Code> >::const_iterator	o=entity_map.find(OID);
	if(o!=entity_map.end())
		return	o->second;

	r_exec::LObject	*new_entity=new	r_exec::LObject();
	new_entity->code(0)=Atom::Object(r_exec::GetOpcode("ent"),1);
	inject(new_entity,NID);
	entity_map[OID]=new_entity;
	return	new_entity;
}

void	DMem::add_entity_map_entry(Code	*entity){

	entity_map[entity->getOID()]=entity;
}