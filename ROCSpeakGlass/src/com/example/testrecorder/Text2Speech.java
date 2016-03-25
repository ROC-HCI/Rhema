package com.example.testrecorder;
import android.content.Context;
import android.speech.tts.TextToSpeech;
import android.util.Log;

public class Text2Speech{
	private TextToSpeech txt2speech;
	
	public Text2Speech(Context cntx) {
		txt2speech = new android.speech.tts.TextToSpeech(cntx, null);
		txt2speech.setSpeechRate((float) 1.5);
	}
	
	public void close(){
		txt2speech.stop();
		txt2speech.shutdown();
	}
	
	public void speakOut(String text){
		txt2speech.speak(text, android.speech.tts.TextToSpeech.QUEUE_FLUSH, null);
	}
	
	public String getHighest(String[] allComponents){
		String[][] data = new String[3][2];
		data[0] = allComponents[4].split("=");
		data[1] = allComponents[6].split("=");
		data[2] = allComponents[8].split("=");
		
		// Angry > Happy
		if(Float.valueOf(data[0][1])>=Float.valueOf(data[1][1])){
			// Angry > Sad
			if(Float.valueOf(data[0][1])>=Float.valueOf(data[2][1])){
				return "Angry";
			} else{
				// Sad > Angry
				return "Sad";
			}
		// Happy > Angry	
		}else{
			// Happy > Sad
			if(Float.valueOf(data[1][1])>=Float.valueOf(data[2][1])){
				return "Happy";
			}else{
				// Sad > Happy
				return "Sad";
			}	
		}
	}
	
	// To produce auditory feedback based on received features
	public void speakResults(String inputRes){
		StringBuilder outputText = new StringBuilder();
		// Split the input into all the face segments
		String[] allFaces = inputRes.split("Left=");
		if(allFaces.length == 2)
			outputText.append("1 face found. ");
		else
			outputText.append((allFaces.length - 1) + " faces found. ");
		// For each face found (starts from 1 because the first part is "Result:")
		for (int i = allFaces.length - 1; i > 0; i--){
			if (allFaces.length>2)
				outputText.append("Face number " + (allFaces.length-i) + ". ");
			String[] allComponents = allFaces[i].split(",");
			
			// Second component is Gender
			outputText.append(allComponents[1].replace('=', ' ') + ". ");
			// Age
			outputText.append(allComponents[2].replace('=', ' ') + " plus minus ");
			outputText.append(allComponents[3].subSequence(allComponents[3].lastIndexOf("=")+1,allComponents[3].length()) + ". ");
			outputText.append("looks " + getHighest(allComponents) + ". ");
			Log.i("Text2Speech", allFaces[i]);
		}
		speakOut(outputText.toString());
	}
}