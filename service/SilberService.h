#include "IService.h"
#include <string.h>
#include <memory>
#include "ServerProcCommunicator.h"
#include "service/ConnectionsInfo.h"
#include "AmApi.h"

namespace am::service
{
	// service implementation based on shared memory messaging
	class SilberService : public IService
	{
	public:
		SilberService(const std::string &shMemName);
		virtual void start() override;
		virtual void stop() override;

	private:
		std::unique_ptr<ServerProcCommunicator> m_server;
		std::unique_ptr<am::AmApi> m_amApi;
		ConnectionsInfo m_connections;
		bool m_isRunning;
	};

}