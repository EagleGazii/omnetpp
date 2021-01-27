simple Node
{
    gates:
        inout gate[];
}

network Network
{
    parameters:
        int nodeCount = 10; //10node - 48 / 50node - 1250 iteration / 100node - 4998 iteration
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
            node[x].gate++ <--> node[y].gate++;			//calisan bir topoloji sadece
        }													//birbiriyle daha fazla baglandi oluyor


      //  node0.gate++ <--> node1.gate++;
      //  node1.gate++ <--> node4.gate++;
      //  node4.gate++ <--> node5.gate++;
      //  node5.gate++ <--> node3.gate++;
      //  node3.gate++ <--> node2.gate++;
      //  node2.gate++ <--> node0.gate++;
      //  node1.gate++ <--> node3.gate++;
      //  node1.gate++ <--> node5.gate++;
}

