/* 
 * File:   handlers.h
 * Author: ycherkasov
 *
 * Created on 25 Январь 2011 г., 13:44
 */

#ifndef HANDLERS_H
#define	HANDLERS_H
#include "common.h"

//---------------------------------------
// read header(packet lenght) ans body (number of symbols)
int readvrec(SOCKET fd, char *bp, size_t len);

/** Every class derived from 'i_packets_handler'
 *  presents client-server handlers logic
 */

class i_packets_handler{
public:
    virtual void client_callback(SOCKET s) = 0;
    virtual void server_callback(SOCKET s) = 0;
};

//-----------------------------------------------------------
/** This class provides simplest client-server logic
 * Fixed lenght messages unsafe send-receive */
class fixed_lenght_buffer : public i_packets_handler{
public:
    static const int ln = 1;

    //-----------------------------------------
    fixed_lenght_buffer(size_t length = ln):_length(length), _buffer(new char[length]){
        bzero(_buffer, _length);
    }

    virtual ~fixed_lenght_buffer(){
        delete[] _buffer;
    }

    //-----------------------------------------
    // Unasafely read fixed-lenght buffer (1 in example)
    virtual void server_callback(SOCKET s) {
        int rc = recv(s, _buffer, _length, 0);
        if (rc <= 0) {
            throw std::runtime_error("server-side: read error");
        }
        printf("received: %s\n", _buffer);

        // send response "2", 1 is length, 0 is flags
        rc = send(s, "2", 1, 0);
        if (rc <= 0) {
            throw std::runtime_error("server-side: send error");
        }
    }

    //-----------------------------------------
    // Unasafely send fixed-lenght buffer (1 in example) and recieve response
    virtual void client_callback(SOCKET s) {
        // send string data "1" to server (1 - length, 0 - flags)
        int rc = send(s, "1", _length, 0);
        if (rc <= 0) {
            throw std::runtime_error("client-side: error send data");
        }

        // receive responce to buf[]
        rc = recv(s, _buffer, 1, 0);
        if (rc <= 0) {
            throw std::runtime_error("client-side: error receive data");
        }
    }
    
private:
    size_t _length;
    char* _buffer;
};

//-----------------------------------------------------------
/** This class provides safe fixed lenght messages transefer */
class safe_buffer : public i_packets_handler{
public:
    static const int ln = 10;
    //-----------------------------------------
    safe_buffer(size_t length = ln):_length(length), _buffer(new char[length]){
        bzero(_buffer, _length);
    }

    virtual ~safe_buffer(){
        delete[] _buffer;
    }

    virtual void server_callback(SOCKET fd) {
        int read = readn(fd);
        if (read != _length){
            throw std::runtime_error("server-side: mismatch data send");
        }
    }

    virtual void client_callback(SOCKET s) {

        // read _length symbols
	while ( fgets( _buffer, _length, stdin ) != NULL )
	{
            std::cout << "send symbols:" << _buffer << std::endl;
            if (send(s, _buffer, _length, 0) < 0)
                throw std::runtime_error("client-side: send error");
            }

    }

protected:
    int readn(SOCKET fd) {
        int cnt = _length;
        char* bp = _buffer;
        // read while fixed lenght reached
        while (cnt > 0) {
            int rc = recv(fd, bp, cnt, 0);
            printf("received: %s\n", bp);
            // read error
            if (rc < 0) {
                if (errno == EINTR){ // interrupted?
                    printf("continue reading with: %s\n", bp);
                    continue; // restart the read
                }
                return -1; // else return error
            }
            // EOF
            if (rc == 0) {
                // read incomplete, return anyway
                printf("read incomplete, return anyway: %s\n", bp);
                return _length - cnt;
            }

            bp += rc;
            cnt -= rc;
        }
        printf("finished with: %s\n", _buffer);
        return _length;
    }

private:
    size_t _length;
    char* _buffer;

};


//-----------------------------------------------------------
/** This class provides safe fixed lenght messages transefer */
class variable_lenght_message : public safe_buffer{
public:
    static const int ln = 100;
    //-----------------------------------------
    variable_lenght_message(size_t length = ln):_length(length), _buffer(new char[length]){
        bzero(_buffer, _length);
    }

    virtual ~variable_lenght_message(){
        delete[] _buffer;
    }

    virtual void server_callback(SOCKET fd) {
        int read = readvrec(fd);
        if (read != _length){
            throw std::runtime_error("server-side: mismatch data recieved");
        }
    }

    virtual void client_callback(SOCKET s) {

        int n;

        struct packet1 {
            u_int32_t reclen;
            char buf[128];
        } mypacket1;
        mypacket1.reclen = sizeof (packet1);

        struct packet2 {
            u_int32_t reclen;
            char buf[64];
        } mypacket2;
        mypacket2.reclen = sizeof (packet1);

        while (true) {
            if (send(s, (char *) &mypacket1, mypacket1.reclen, 0) < 0)
                throw std::runtime_error("client-side: send failure");
            if (send(s, (char *) &mypacket2, mypacket2.reclen, 0) < 0)
                throw std::runtime_error("client-side: send failure");

        }
    }

protected:

    int readvrec(SOCKET fd) {
        // Retrieve message header (the length of the record)
        u_int32_t reclen;
        char* bp  = _buffer;
        int rc = recv(fd, (char *)&reclen, sizeof ( u_int32_t), 0);
        if (rc != sizeof ( u_int32_t))
            return rc < 0 ? -1 : 0;

        // real message lenght
        reclen = ntohl(reclen);

        //Not enough room for the record --
        //discard it and return an error
        if (reclen > _length) {
            int len = _length;
            while (reclen > 0) {

                // read max available symbols
                rc = readn(fd);
                if (rc != len) {
                    return rc < 0 ? -1 : 0;
                }
                // read remains
                reclen -= len;
                if (reclen < len)
                    len = reclen;
            }
            return -1;
        }

        // ok, read message body
        rc = readn(fd);
        if (rc != reclen)
            return rc < 0 ? -1 : 0;
        return rc;
    }

private:
    size_t _length;
    char* _buffer;

};



#endif	/* HANDLERS_H */

