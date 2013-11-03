/*
 * uihelper.h
 *
 *  Created on: 29 Aug 2013
 *      Author: DTHOZBAYBURTLU
 */
#ifndef __UI_HELPER_H_
#define __UI_HELPER_H_

#include <iostream>
#include <string>
#include <vector>

#include "mongoose.h"
#include "dbconnector.h"

class UIHelper{

private:

    //singleton instance:
    static UIHelper* instance;
    UIHelper();

    //mongoose socket handlers:
    struct mg_context *ctx;
    static void websocket_ready_handler(mg_connection*);
    static int websocket_data_handler(mg_connection*, int, char *, size_t);

    //internal methods
    void initiateSocket();
    
    //internal attributes
    DBConnector* dbconnector;
    
public:
    static UIHelper* getInstance();  
    bool isActive();

    //To allow polymorphic deletion
    virtual ~UIHelper();
};

#endif

