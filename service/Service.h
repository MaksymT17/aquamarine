#pragma once
#include"IService.h"
namespace service {

	class Service : public IService
	{
	public:
		Service() = default;
		virtual ~Service() {};

		void start() override;
		void stop()override;
	};

}