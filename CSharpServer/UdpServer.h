#pragma once

#include "Endpoint.h"

#include <server/asio/udp_server.h>

namespace CSharpServer {

    ref class UdpServer;

    class UdpServerEx : public CppServer::Asio::UDPServer
    {
    public:
        using CppServer::Asio::UDPServer::UDPServer;

        gcroot<UdpServer^> root;

        void onStarted() override;
        void onStopped() override;
        void onReceived(const asio::ip::udp::endpoint& endpoint, const void* buffer, size_t size) override;
        void onSent(const asio::ip::udp::endpoint& endpoint, size_t sent) override;
        void onError(int error, const std::string& category, const std::string& message) override;
    };

    //! UDP server
    /*!
        UDP server is used to send or multicast datagrams to UDP endpoints.
    */
    public ref class UdpServer
    {
    public:
        //! Initialize UDP server with a given service, protocol and port number
        /*!
            \param service - Service
            \param protocol - Protocol type
            \param port - Port number
        */
        UdpServer(Service^ service, CSharpServer::InternetProtocol protocol, int port);
        //! Initialize UDP server with a given service, IP address and port number
        /*!
            \param service - Service
            \param address - IP address
            \param port - Port number
        */
        UdpServer(Service^ service, String^ address, int port);
        //! Initialize UDP server with a given Asio service and UDP endpoint
        /*!
            \param service - Asio service
            \param endpoint - Server UDP endpoint
        */
        UdpServer(Service^ service, UdpEndpoint^ endpoint);
        ~UdpServer() { this->!UdpServer(); }

        //! Get the service
        property Service^ Service { CSharpServer::Service^ get() { return _service; } }

        //! Get the number datagrams sent by the server
        property long long DatagramsSent { long long get() { return (long long)_server->get()->datagrams_sent(); } }
        //! Get the number datagrams received by the server
        property long long DatagramsReceived { long long get() { return (long long)_server->get()->datagrams_received(); } }
        //! Get the number of bytes pending sent by the server
        property long long BytesPending { long long get() { return (long long)_server->get()->bytes_pending(); } }
        //! Get the number of bytes sent by the server
        property long long BytesSent { long long get() { return (long long)_server->get()->bytes_sent(); } }
        //! Get the number of bytes received by the server
        property long long BytesReceived { long long get() { return (long long)_server->get()->bytes_received(); } }

        //! Get the option: reuse address
        property bool OptionReuseAddress { bool get() { return _server->get()->option_reuse_address(); } }
        //! Get the option: reuse port
        property bool OptionReusePort { bool get() { return _server->get()->option_reuse_port(); } }
        //! Get the option: receive buffer size
        property long OptionReceiveBufferSize { long get() { return (long)_server->get()->option_receive_buffer_size(); } }
        //! Get the option: send buffer size
        property long OptionSendBufferSize { long get() { return (long)_server->get()->option_send_buffer_size(); } }

        //! Is the server started?
        property bool IsStarted { bool get() { return _server->get()->IsStarted(); } }

        //! Start the server
        /*!
            \return 'true' if the server was successfully started, 'false' if the server failed to start
        */
        bool Start() { return _server->get()->Start(); }
        //! Start the server with a given multicast IP address and port number
        /*!
            \param multicast_address - Multicast IP address
            \param multicast_port - Multicast port number

            \return 'true' if the server was successfully started, 'false' if the server failed to start
        */
        bool Start(String^ multicast_address, int multicast_port) { return _server->get()->Start(marshal_as<std::string>(multicast_address), multicast_port); }
        //! Start the server with a given multicast endpoint
        /*!
            \param multicast_endpoint - Multicast UDP endpoint

            \return 'true' if the server was successfully started, 'false' if the server failed to start
        */
        bool Start(UdpEndpoint^ multicast_endpoint) { return _server->get()->Start(multicast_endpoint->_endpoint.Value); }
        //! Stop the server
        /*!
            \return 'true' if the server was successfully stopped, 'false' if the server is already stopped
        */
        bool Stop() { return _server->get()->Stop(); }
        //! Restart the server
        /*!
            \return 'true' if the server was successfully restarted, 'false' if the server failed to restart
        */
        bool Restart() { return _server->get()->Restart(); }

        //! Multicast datagram to the prepared mulicast endpoint
        /*!
            \param buffer - Datagram buffer to multicast
            \return 'true' if the datagram was successfully multicasted, 'false' if the datagram was not multicasted
        */
        bool Multicast(array<Byte>^ buffer) { return Multicast(buffer, 0, buffer->Length); }
        //! Multicast a datagram to the prepared mulicast endpoint
        /*!
            \param buffer - Datagram buffer to multicast
            \param offset - Datagram buffer offset
            \param size - Datagram buffer size
            \return 'true' if the datagram was successfully multicasted, 'false' if the datagram was not multicasted
        */
        bool Multicast(array<Byte>^ buffer, long long offset, long long size)
        {
            pin_ptr<Byte> ptr = &buffer[buffer->GetLowerBound(0) + (int)offset];
            return (long long)_server->get()->Multicast(ptr, size);
        }
        //! Multicast text to the prepared mulicast endpoint
        /*!
            \param text - Text string to multicast
            \return 'true' if the text was successfully multicasted, 'false' if the text was not multicasted
        */
        bool Multicast(String^ text)
        {
            std::string temp = marshal_as<std::string>(text);
            return (long long)_server->get()->Multicast(temp.data(), temp.size());
        }

        //! Send a datagram into the given endpoint
        /*!
            \param endpoint - Endpoint to send
            \param buffer - Datagram buffer to send
            \return 'true' if the datagram was successfully multicasted, 'false' if the datagram was not multicasted
        */
        bool Send(UdpEndpoint^ endpoint, array<Byte>^ buffer) { return Send(endpoint, buffer, 0, buffer->Length); }
        //! Send a datagram into the given endpoint
        /*!
            \param endpoint - Endpoint to send
            \param buffer - Datagram buffer to send
            \param offset - Datagram buffer offset
            \param size - Datagram buffer size
            \return 'true' if the datagram was successfully multicasted, 'false' if the datagram was not multicasted
        */
        bool Send(UdpEndpoint^ endpoint, array<Byte>^ buffer, long long offset, long long size)
        {
            pin_ptr<Byte> ptr = &buffer[buffer->GetLowerBound(0) + (int)offset];
            return (long long)_server->get()->Send(endpoint->_endpoint.Value, ptr, size);
        }
        //! Send a text string into the given endpoint
        /*!
            \param endpoint - Endpoint to send
            \param text - Text string to send
            \return 'true' if the datagram was successfully multicasted, 'false' if the datagram was not multicasted
        */
        bool Send(UdpEndpoint^ endpoint, String^ text)
        {
            std::string temp = marshal_as<std::string>(text);
            return (long long)_server->get()->Send(endpoint->_endpoint.Value, temp.data(), temp.size());
        }

        //! Setup option: reuse address
        /*!
            This option will enable/disable SO_REUSEADDR if the OS support this feature.

            \param enable - Enable/disable option
        */
        void SetupReuseAddress(bool enable) { return _server->get()->SetupReuseAddress(enable); }
        //! Setup option: reuse port
        /*!
            This option will enable/disable SO_REUSEPORT if the OS support this feature.

            \param enable - Enable/disable option
        */
        void SetupReusePort(bool enable) { return _server->get()->SetupReusePort(enable); }
        //! Setup option: receive buffer size
        /*!
            This option will setup SO_RCVBUF if the OS support this feature.

            \param size - Receive buffer size
        */
        void SetupReceiveBufferSize(long size) { return _server->get()->SetupReceiveBufferSize(size); }
        //! Setup option: send buffer size
        /*!
            This option will setup SO_SNDBUF if the OS support this feature.

            \param size - Send buffer size
        */
        void SetupSendBufferSize(long size) { return _server->get()->SetupSendBufferSize(size); }

    protected:
        //! Handle server started notification
        virtual void OnStarted() {}
        //! Handle server stopped notification
        virtual void OnStopped() {}

        //! Handle datagram received notification
        /*!
            Notification is called when another datagram was received from
            some endpoint.

            \param endpoint - Received endpoint
            \param buffer - Received datagram buffer
        */
        virtual void OnReceived(UdpEndpoint^ endpoint, array<Byte>^ buffer) {}
        //! Handle datagram sent notification
        /*!
            Notification is called when a datagram was sent to the client.

            This handler could be used to send another datagram to the client
            for instance when the pending size is zero.

            \param endpoint - Endpoint of sent datagram
            \param sent - Size of sent datagram buffer
        */
        virtual void OnSent(UdpEndpoint^ endpoint, long long sent) {}

        //! Handle error notification
        /*!
            \param error - Error code
            \param category - Error category
            \param message - Error message
        */
        virtual void OnError(int error, String^ category, String^ message) {}

    internal:
        void InternalOnStarted() { OnStarted(); }
        void InternalOnStopped() { OnStopped(); }
        void InternalOnReceived(UdpEndpoint^ endpoint, array<Byte>^ buffer) { OnReceived(endpoint, buffer); }
        void InternalOnSent(UdpEndpoint^ endpoint, long long sent) { OnSent(endpoint, sent); }
        void InternalOnError(int error, String^ category, String^ message) { OnError(error, category, message); }

    protected:
        !UdpServer() { _server.Release(); }

    private:
        CSharpServer::Service^ _service;
        Embedded<std::shared_ptr<UdpServerEx>> _server;

    internal:
        UdpEndpoint^ _recive_endpoint;
        UdpEndpoint^ _send_endpoint;
    };

}
