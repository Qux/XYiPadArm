class Metro  {
  private float previousTime;
  private float interval;
  
  Metro(float i) {
    interval = i;
    previousTime = millis();
  }
  
  void setInterval(float i) {
    interval = i; 
  }
  
  boolean bang() {
    float currentTime = millis();
    float dt = currentTime - previousTime;
    if( interval < dt) {
      float margin = dt - interval;
      previousTime = (currentTime - margin);
      return true;
    } else {
      return false;
    }
  }
}
