/*
Djimgou Patrick Virgile 

 */
// zufaliige Eintrag zum Simulieren die kommunikation zwisschen die diese Socket Client und der Server
 
String neu_de_eintrag(String Arduino_Name , int temp ,int Actual_temp,int tw){
  String Data_zu_senden ;
  
  // Temp%arduino=ARD21&zeit=2015-12-05 00:00:00&temp=35&tcTemp=65&tw=55
  
  Data_zu_senden ="Temp%arduino="+Arduino_Name+
  "&zeit=2015-12-05 00:00:00&"  // Datum ....für diese beispiel , es wird fest
  "temp="+temp+
  "&tcTemp="+Actual_temp+
  "&tw="+tw;
  
 return Data_zu_senden; 
 
 }
 

String neu_db_data_eintrag(String Arduino_Name , int gs_nr ,int x,int y,int error ){
  String Data_zu_senden;
  
  Data_zu_senden = "Data_a%id=1&arduino="+Arduino_Name+
  "&datetime=2015-12-05 00:00:00&device_id=ARD20&gs_nr="+gs_nr+
  "&x="+x+
  "&y="+y+
  "&error_count="+error+
  "&number_tc=55&temperature=650&heatup=654&value=99";
 
  return Data_zu_senden; 
 
}

void zufall_data(){ random(0,10);}

