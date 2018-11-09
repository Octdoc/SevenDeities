#pragma once

#include "hcs_input.h"

#define ClearStruct(s) memset(&s, 0, sizeof(s))

//!! makes members after it private !!
#define SHARED_ONLY(TYPE) public:using P = std::shared_ptr<TYPE>;using W = std::weak_ptr<TYPE>;using U = std::unique_ptr<TYPE>;private:friend std::_Ref_count_obj<TYPE>;TYPE(TYPE&) = delete;

//!! makes members after it private !!
#define SHARED_TOO(TYPE) public:using P = std::shared_ptr<TYPE>;using W = std::weak_ptr<TYPE>;using U = std::unique_ptr<TYPE>;private:friend std::_Ref_count_obj<TYPE>;

namespace octdoc
{
	namespace hcs
	{
	}
}