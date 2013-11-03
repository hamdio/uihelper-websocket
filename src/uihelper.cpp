/*
 * uihelper.cpp
 *
 *  Created on: 29 Aug 2013
 *      Author: DTHOZBAYBURTLU
 */
#include "string.h"
#include "uihelper.h"
#include <exception>

UIHelper* UIHelper::instance = NULL;

UIHelper::UIHelper() {
    
	  //create web socket
    initiateSocket();

    //init database
    dbconnector = DBConnector::getInstance();
    std::string dbFile = "test/headend_data_indexation.db";
    dbconnector->loadDb(&dbFile);

}

UIHelper::~UIHelper() {

    mg_stop(this->ctx);
    delete dbconnector;
    instance = NULL;
    
    std::cout << "UIHelper destructor end\n";
}

UIHelper* UIHelper::getInstance() {
    if(instance == NULL)
        instance = new UIHelper();
    return instance;
}

bool UIHelper::isActive() {
    return true;
}

void UIHelper::initiateSocket() {

    struct mg_callbacks callbacks;
    const char *options[] = {
      "listening_ports", "6100",
      "document_root", "test/websocket_html_root",
      NULL
    };

    memset(&callbacks, 0, sizeof(callbacks));
    callbacks.websocket_ready = &this->websocket_ready_handler;
    callbacks.websocket_data = &this->websocket_data_handler;
    this->ctx = mg_start(&callbacks, NULL, options);
}

void UIHelper::websocket_ready_handler(mg_connection *conn) {
  //static const char *message = "server ready";
  //mg_websocket_write(conn, WEBSOCKET_OPCODE_TEXT, message, strlen(message));
  printf("server ready\n");
}

// Arguments:
//   flags: first byte of websocket frame, see websocket RFC,
//          http://tools.ietf.org/html/rfc6455, section 5.2
//   data, data_len: payload data. Mask, if any, is already applied.
int UIHelper::websocket_data_handler(mg_connection *conn, int flags,
                                  char *data, size_t data_len) {
  (void) flags; // Unused
  
  // Returning zero means stoping websocket conversation.
  // Close the conversation if client has sent us "exit" string.
  if (memcmp(data, "exit", 4) == 0)
    return 0;

  if (strcmp(mg_get_request_info(conn)->uri,"/searchdbquery") == 0)
  {
	  string message(data,data_len);
	  string query;

	  Json::Value jValue;
	  Json::Reader JReader;

    printf("Message: %s\n",message.c_str() );

	  if( JReader.parse(message, jValue) ){
		  try{
			  query = jValue.get("query","").asString();
		  }
		  catch (exception &e){
			  cout << "Exception!:" << e.what() << endl;
		  }
	  }
	  else {
		  printf("Parse error :%s\n",JReader.getFormatedErrorMessages().c_str());
	  }

	  if (query.length()){
		  Json::Value result;
		  instance->dbconnector->query(&query, &result);

		  Json::StyledWriter writer;
		  string result_str = writer.write(result);
		  mg_websocket_write(conn, WEBSOCKET_OPCODE_TEXT, result_str.c_str(), result_str.length()+1);
	  }

  }
  else
  {
	  //TODO: Echo temporarily
	  mg_websocket_write(conn, WEBSOCKET_OPCODE_TEXT, data, data_len);
  }

  printf("data returned for %s \n",mg_get_request_info(conn)->uri);
  return 1;
}
