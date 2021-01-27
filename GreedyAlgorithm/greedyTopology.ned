
simple Node
{
    parameters:
        @display("i=block/routing");
    gates:
        inout gate[];
}

network Network
{
    parameters:
        int nodeCount = 8;
        int variable = int(nodeCount / 2)


        @display("bgb=699,238");
    submodules:
        node[nodeCount]: Node;
    
    connections:
       //  for x=0..(nodeCount-1), for y=0..(nodeCount-1), if x>y, if y % 4 == 0 || y % 3 == 0  {
        //     node[x].gate++ <--> {  delay = 0ms; } <--> node[y].gate++;			//calisan bir topoloji sadece
       //  }
       for i = 0..(nodeCount -2){
        	node[i].gate++ <--> {  delay = 0ms; } <--> node[i+1].gate++;   
           
       }
       
		node[0].gate++ <--> {  delay = 0ms; } <--> node[nodeCount-1].gate++; 
		
		for i = 1..(variable-1){
		 	node[i].gate++ <--> {  delay = 0ms; } <--> node[nodeCount - i].gate++; 	   
		}
		for i = 2..(variable -1){
		 	   node[i].gate++ <--> {  delay = 0ms; } <--> node[nodeCount - i + 1].gate++;
		}
		

}
