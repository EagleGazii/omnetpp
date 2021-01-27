//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
//
simple DenemeNode
{
    parameters:
        @display("i=block/routing");
    gates:
        inout gate[];
}

network DenemeNetwork
{
    parameters:
        int nodeCount = 8;

        @display("bgb=699,238");
    submodules:
        denemeNode: DenemeNode {
            @display("p=287,39;i=device/pc");
        }
        denemeNode3: DenemeNode {
            @display("p=120,39;i=device/pc");
        }
        denemeNode4: DenemeNode {
            @display("p=120,210;i=device/pc");
        }
        denemeNode1: DenemeNode {
            @display("p=287,210;i=device/pc");
        }
        denemeNode2: DenemeNode {
            @display("p=426,210;i=device/pc");
        }
        denemeNode5: DenemeNode {
            @display("p=572,210;i=device/pc");
        }
        denemeNode6: DenemeNode {
            @display("p=426,39;i=device/pc");
        }
        denemeNode7: DenemeNode {
            @display("p=572,39;i=device/pc");
        }
    connections:




        denemeNode3.gate++ <--> {  delay = 10ms; } <--> denemeNode4.gate++;
        denemeNode.gate++ <--> {  delay = 10ms; } <--> denemeNode3.gate++;
        denemeNode.gate++ <--> {  delay = 10ms; } <--> denemeNode4.gate++;
        denemeNode4.gate++ <--> {  delay = 10ms; } <--> denemeNode1.gate++;
        denemeNode1.gate++ <--> {  delay = 10ms; } <--> denemeNode6.gate++;
        denemeNode6.gate++ <--> {  delay = 10ms; } <--> denemeNode2.gate++;
        denemeNode2.gate++ <--> {  delay = 10ms; } <--> denemeNode1.gate++;
        denemeNode1.gate++ <--> {  delay = 10ms; } <--> denemeNode.gate++;
        denemeNode.gate++ <--> {  delay = 10ms; } <--> denemeNode6.gate++;
        denemeNode7.gate++ <--> {  delay = 10ms; } <--> denemeNode6.gate++;
        denemeNode7.gate++ <--> {  delay = 10ms; } <--> denemeNode2.gate++;
        denemeNode2.gate++ <--> {  delay = 10ms; } <--> denemeNode5.gate++;
        denemeNode5.gate++ <--> {  delay = 10ms; } <--> denemeNode7.gate++;
}
