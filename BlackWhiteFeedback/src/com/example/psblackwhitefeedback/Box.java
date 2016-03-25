package com.example.psblackwhitefeedback;

public class Box {
	
	double A, R, G, B;
	int r1, r2, r3, r4;
	int currentState, futureState;	//Red = 0, Green = 1
	int speedValue, volumeValue;

	public Box(int speedValue, int volumeValue, int r1, int r2, int r3, int r4, double R, double G, double B) {
		this.speedValue = speedValue;
		this.volumeValue = volumeValue;
		this.R = R;
		this.G = G;
		this.B = B;
		this.r1 = r1;
		this.r2 = r2;
		this.r3 = r3;
		this.r4 = r4;
	}
	
} //end class
