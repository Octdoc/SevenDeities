#pragma once

#include "hcs_input.h"

#define ClearStruct(s) memset(&s, 0, sizeof(s))

//!! makes members after it private !!
#define SHARED_ONLY(TYPE) public:using P = std::shared_ptr<TYPE>;using W = std::weak_ptr<TYPE>;using U = std::unique_ptr<TYPE>;private:friend std::_Ref_count_obj<TYPE>;TYPE(TYPE&) = delete;

namespace hcs
{

	class MessageHandler
	{
		SHARED_ONLY(MessageHandler);

	protected:
		MessageHandler() = default;

	public:
		virtual void HandleMessage(Input& input) = 0;
	};

	class AutoUpdater
	{
		SHARED_ONLY(AutoUpdater);

	protected:
		AutoUpdater() = default;

	public:
		virtual void Update(float deltaTime, float totalTime) = 0;
	};
}