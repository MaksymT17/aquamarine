#pragma once

namespace am {
	namespace service {
		class IService
		{
		public:
			virtual ~IService() = default;

			virtual void start() = 0;
			virtual void stop() = 0;
		};

	}
}