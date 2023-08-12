//
//  CBridge.c
//  resourcesss_look
//
//  Created by Валерий on 11.08.2023.
//

#include "CClientSocket.h"

void cGetRumMemoryInfo(char buffer[]) {
    getRamInfo(SERVER_SOCKET_FILE_PATH, buffer);
}
