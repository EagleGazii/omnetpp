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
simple Node
{
    gates:
        inout gate[];
}
network Network
{
        parameters:
        int nodeCount = 6;
        @display("bgb=699,238");
    submodules:
        node[nodeCount]: Node {
      //  node0: Node {
      //      @display("p=50,32");
      //  }
      //  node1: Node {
      //      @display("p=158,32");
      //  }
      //  node2: Node {
      //      @display("p=50,129");
      //  }
      //  node3: Node {
      //      @display("p=158,129");
      //  }
      //  node4: Node {
      //    @display("p=271,32");
      //  }
      //  node5: Node {
      //      @display("p=271,129");
      //  }
            @display("i=device/pc");
        }
    connections:

 //    for i=0..(nodeCount-1),  for j=0..(nodeCount-1), if j>i ,if j % 4 == 0 || j % 3 == 0 && i % 2 == 1 { //7 nodetan sonra calisan
     //   	node[i].gate++ <--> Channel <--> node[j].gate++;												// bir topoloji

        // }

        for x=0..(nodeCount-1), for y=0..(nodeCount-1), if x>y, if y % 4 == 0 || y % 3 == 0 {
            node[x].gate++ <--> {delay=10ms;} <--> node[y].gate++;			//calisan bir topoloji sadece
        }	
        
}
