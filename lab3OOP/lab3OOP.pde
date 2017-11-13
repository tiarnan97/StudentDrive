
float[] rainFall = {45, 37, 55, 27, 38, 50, 79, 48, 104, 31, 100, 58};
String[] months = {"Jan", "Feb", "March", "April", "May"
  , "June", "July", "August", "Sept", "Oct", "Nov", "Dec" };
  
 
void setup() {
  size(500, 500);
  strokeWeight(6);
  smooth();
}
 
void draw() {
  background(255);
  float lineWidth = (float) width/(rainFall.length-1);
  for (int i=0; i<rainFall.length-1; i++) {
    line(i*lineWidth, 
    map(rainFall[i],0,150,height,width), 
    (i+1)*lineWidth, 
    map(rainFall[i+1],0,150,height,width));
  }
}
 