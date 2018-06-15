//
// Copyright (c) 2013-2016 Pavel Medvedev. All rights reserved.
//
// This file is part of v8pp (https://github.com/pmed/v8pp) project.
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef V8PP_CONTEXT_HPP_INCLUDED
#define V8PP_CONTEXT_HPP_INCLUDED

#include <string>
#include <map>
#include "../log.h"
#include <v8.h>
using namespace v8;

#include "convert.hpp"

namespace v8pp {

class module;

template<typename T, typename Traits>
class class_;

/// V8 isolate and context wrapper
class context
{
public:
	/// Create context with optional existing v8::Isolate
	/// and v8::ArrayBuffer::Allocator
	explicit context(v8::Isolate* isolate = nullptr);
	~context();

	/// V8 isolate associated with this context
	v8::Isolate* isolate() { return isolate_; }

	/// Library search path
	std::string const& lib_path() const { return lib_path_; }

	/// Set new library search path
	void set_lib_path(std::string const& lib_path) { lib_path_ = lib_path; }

	/// Set a V8 value in the context global object with specified name
	context& set(char const* name, v8::Local<v8::Value> value);

	/// Set module to the context global object
	context& set(char const *name, module& m);

	/// Set class to the context global object
	template<typename T, typename Traits>
	context& set(char const* name, class_<T, Traits>& cl)
	{
		v8::HandleScope scope(isolate_);
		cl.class_function_template()->SetClassName(v8pp::to_v8(isolate_, name));
		return set(name, cl.js_function_template()->GetFunction(isolate_->GetCurrentContext()).ToLocalChecked());
	}

    void run_script(Isolate *isolate, const char *name, bool print_result, bool report_exceptions);

private:
	bool own_isolate_;
	v8::Isolate* isolate_;
	v8::Global<v8::Context> impl_;

	struct dynamic_module;
	using dynamic_modules = std::map<std::string, dynamic_module>;

	static void load_module(v8::FunctionCallbackInfo<v8::Value> const& args);
	static void require_module(v8::FunctionCallbackInfo<v8::Value> const& args);
//	static void run_file(v8::FunctionCallbackInfo<v8::Value> const& args);

	dynamic_modules modules_;
	std::string lib_path_;
    };

} // namespace v8pp

#endif // V8PP_CONTEXT_HPP_INCLUDED