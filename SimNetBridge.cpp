#include <iostream>
#include <vector>
#include <string>

using namespace std;

struct ForwardingTableStruct {
    string addr;
    string port;
};

void PC1(string frame) {
    //addr: 0101
    if (frame.substr(0, 4) == "0101")
        cout << "PC1 received data: " << frame.substr(8) << endl;
}
void PC2(string frame) {
    //addr: 0202
    if (frame.substr(0, 4) == "0202")
        cout << "PC2 received data: " << frame.substr(8) << endl;
}
void PC3(string frame) {
    //addr: 0303
    if (frame.substr(0, 4) == "0303")
        cout << "PC3 received data: " << frame.substr(8) << endl;
}
void portToPC(string port, string frame) {
    if (port == "Port1") PC1(frame);
    else if (port == "Port2") PC2(frame);
    else if (port == "Port3") PC3(frame);
    else if (port == "Broadcast") {
        PC1(frame);
        PC2(frame);
        PC3(frame);
    }
    else cout << "Error: No such port" << endl;
}

void Forward(string inComingPort, string frame, vector<ForwardingTableStruct> &forwardingTable) {
    string currentDest = frame.substr(0, 4);
    string currentSrc = frame.substr(4, 4);
    cout << "NetBridge: " << currentSrc << " -> " << currentDest << " : " << frame.substr(8) << endl;
    //try to forward
    //search in forwarding table
    for (int timer = 0; timer < forwardingTable.size(); timer++) {
        if (forwardingTable[timer].addr == currentDest) {
            portToPC(forwardingTable[timer].port, frame);
            cout << "Forwarding to " << forwardingTable[timer].port << " successfully." << endl;
            return;
        }
    }
    //not found, broadcast and build table
    cout << "No such addr in forwarding table, broadcasting." << endl;
    portToPC("Broadcast", frame);
    ForwardingTableStruct newRow;
    newRow.addr = currentSrc;
    newRow.port = inComingPort;
    forwardingTable.push_back(newRow);
    cout << "Add new row to forwarding table: " << newRow.addr << " -> " << newRow.port << endl;
}

string EncapsulationFrame(string dstAddr, string srcAddr, string data) {
    string frame = dstAddr + srcAddr + data;
    cout << "Encapsulated frame: " << frame << endl;
    return frame;
}

//frame struct: dst addr[4] + src addr[4] + data

int main() {
    vector<ForwardingTableStruct> forwardingTable;
    //use strings to simulate files

    string srcDevice, dstDevice, data;
    while (true) {
        cout << "NetBridge Simulation. Input 'exit' to quit." << endl;
        cout << "Input your source device (PC1, PC2, PC3): ";
        cin >> srcDevice;
        if (srcDevice == "exit") break;
        if (srcDevice != "PC1" && srcDevice != "PC2" && srcDevice != "PC3") {
            cout << "Error: No such device" << endl;
            continue;
        }
        cout << "Input your destination device (PC1, PC2, PC3): ";
        cin >> dstDevice;
        if (dstDevice != "PC1" && dstDevice != "PC2" && dstDevice != "PC3") {
            cout << "Error: No such device" << endl;
            continue;
        }
        cout << "Input your data: ";
        cin >> data;
        //translate PCn to port and address
        string srcPort;
        if (srcDevice == "PC1") srcPort = "Port1";
        else if (srcDevice == "PC2") srcPort = "Port2";
        else if (srcDevice == "PC3") srcPort = "Port3";
        string dstAddr, srcAddr;
        if (dstDevice == "PC1") dstAddr = "0101";
        else if (dstDevice == "PC2") dstAddr = "0202";
        else if (dstDevice == "PC3") dstAddr = "0303";
        if (srcDevice == "PC1") srcAddr = "0101";
        else if (srcDevice == "PC2") srcAddr = "0202";
        else if (srcDevice == "PC3") srcAddr = "0303";
        //forward
        string frame = EncapsulationFrame(dstAddr, srcAddr, data);
        Forward(srcPort, frame, forwardingTable);

    }
}
