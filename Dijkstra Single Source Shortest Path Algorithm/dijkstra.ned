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
simple Node{
    parameters:
        @display("i=device/pc");
    gates:
        inout gate[];
}
network Network{
    parameters:
        int nodeCount = 16; //node sayisi
        @display("bgb=699,238");
    submodules:
        node[nodeCount]: Node {
            @display("i=device/pc");
        }
    connections:
                 for x=0..(nodeCount-1), for y=0..(nodeCount-1), if x>y, if y % 4 == 0 || y % 3 == 0  {
                     node[x].gate++ <--> {  delay = 0ms; } <--> node[y].gate++;			//calisan bir topoloji
                 }
}
