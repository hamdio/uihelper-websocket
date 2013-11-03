target-y = uihelper

uihelper_files-y = \
	src/sqlite3.c \
	src/mongoose.c \
	src/lib_json/json_reader.cpp \
	src/lib_json/json_value.cpp \
	src/lib_json/json_writer.cpp \
	src/uihelper.cpp \
	src/dbconnector.cpp \
	src/main.cpp

uihelper_includes-y = ./ \
	inc

uihelper_cflags-y += -Wno-unused-value -Wno-unused-parameter \
	-DTHREADSAFE=1 \
	-DSQLITE_ENABLE_FTS3 \
	-DSQLITE_ENABLE_FTS3_PARENTHESIS \
	-DNO_SSL \
	-DUSE_WEBSOCKET

uihelper_ldflags-y += -lpthread -ldl

include lib/Makefile.lib
