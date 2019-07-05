#include "../src/polkadot.h"

int main(int argc, char *argv[]) {

    // Create and connect app layer
    JsonRpcParams params;
    params.jsonrpcVersion = "2.0";

    EasyLogger logger;
    CJsonRpc jsonRpc(CWebSocketClient::getInstance(&logger), &logger, params);

    CPolkaApi app(&logger, &jsonRpc);
    app.connect();

    // Subscribe to block number updates
    bool done = false;
    app.subscribeEraAndSession([&](Era era, Session session) {
        cout << endl << "currentEra: " << era.currentEra << endl << endl;
        cout << endl << "eraProgress: " << era.eraProgress << endl << endl;
        cout << endl << "sessionIndex: " << session.sessionIndex << endl << endl;
        cout << endl << "sessionProgress: " << session.sessionProgress << endl << endl;

        done = true;
    });

    // Wait until block number update arrives
    while (!done)
        usleep(2000);
        
    // Unsubscribe and close connection
    app.unsubscribeEraAndSession();
    app.disconnect();

    cout << "success" << endl;

    return 0;
}