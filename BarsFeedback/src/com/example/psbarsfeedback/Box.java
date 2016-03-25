package com.example.psbarsfeedback;

public class Box {
	
	double R, G, B;
	int r1, r2, r3, r4;
	int currentState, futureState;	//Red = 0, Green = 1

	public Box(int currentState, int futureState, int r1, int r2, int r3, int r4, double R, double G, double B) {
		this.currentState = currentState;
		this.futureState = futureState;
		this.R = R;
		this.G = G;
		this.B = B;
		this.r1 = r1;
		this.r2 = r2;
		this.r3 = r3;
		this.r4 = r4;
	}
	
} //end class
