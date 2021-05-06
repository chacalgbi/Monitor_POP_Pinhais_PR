char ssid[30];   // Rede WiFi
char pw[30];     // Senha da Rede WiFi
char device[30]; // Nome do dispositivo
char auth[] = "vIYy5i9PiCN8IgKbiLwGjeneMacfZmHM";

float ping_ms = 0;
float temp_dht = 0;
float umi_dht = 0; 
float bat1   = 0;
float bat2   = 0;
float tensao1   = 0;
float tensao2   = 0; 
float tensaoAC1 = 0;
float tensaoAC2 = 0;
float tensaoAC3 = 0;
float tensaoAC4 = 0;

int temp_int = 0;
int limite_AC = 0;
int limite_AC_mais = 0;
int limite_AC_mono = 0;
int limite_AC_mais_mono = 0;
int x = 0;
int tensao_bateria = 0;
int troca_display = 0;

bool error_conect = false;
bool limite_AC1_trava = false;
bool limite_AC2_trava = false;
bool limite_AC3_trava = false;
bool limite_AC4_trava = false;
bool limite_AC_trava_mono = false;
bool rele1_estado = false;
bool rele2_estado = false;
bool modo_teste = true;
bool ping_ = false;
bool iniciar = false;
bool modo_config = false;
bool bateria_trava = false;

String temp = "";
String device_str = "";
String pw_str = "";
String ssid_str = "";
String AC_str1    = "";
String AC_str2    = "";
String AC_str3    = "";
String AC_str4    = "";
String bat1_str   = "";
String bat2_str   = "";
String dht_str    = "";
String linha1 = "";
String linha2 = "";
String linha3 = "";
String linha4 = "";
String IP = "";
String sinal_wifi = "";
String wifi_on_off = "";
String RAM_livre = "";
String Programa = "";
String Free_Prog = "";
String Chip_Size = "";
String Freq_proc = "";
String conexao = "";
String currentTime = "";
String hora = "";
String data_ = "";
String dia_semana = "";
String app_temp_ext = "Aguarde...";
String app_tensoes = "Aguarde...";

// Atualização via OTA WEBServer

const char* host = "Monitor_Tensoes";

const char* loginIndex = 
 "<form name='loginForm'>"
    "<table width='20%' bgcolor='A09F9F' align='center'>"
        "<tr>"
            "<td colspan=2>"
                "<center><font size=4><b>ESP32 - identifique-se</b></font></center>"
                "<br>"
            "</td>"
            "<br>"
            "<br>"
        "</tr>"
        "<td>Login:</td>"
        "<td><input type='text' size=25 name='userid'><br></td>"
        "</tr>"
        "<br>"
        "<br>"
        "<tr>"
            "<td>Senha:</td>"
            "<td><input type='Password' size=25 name='pwd'><br></td>"
            "<br>"
            "<br>"
        "</tr>"
        "<tr>"
            "<td><input type='submit' onclick='check(this.form)' value='Identificar'></td>"
        "</tr>"
    "</table>"
"</form>"
"<script>"
    "function check(form)"
    "{"
    "if(form.userid.value=='admin' && form.pwd.value=='10203040')"
    "{"
    "window.open('/serverIndex')"
    "}"
    "else"
    "{"
    " alert('Login ou senha inválidos')"
    "}"
    "}"
"</script>";
  
const char* serverIndex = 
"<script src='https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js'></script>"
"<form method='POST' action='#' enctype='multipart/form-data' id='upload_form'>"
   "<input type='file' name='update'>"
        "<input type='submit' value='Update'>"
    "</form>"
 "<div id='prg'>Progresso: 0%</div>"
 "<script>"
  "$('form').submit(function(e){"
  "e.preventDefault();"
  "var form = $('#upload_form')[0];"
  "var data = new FormData(form);"
  " $.ajax({"
  "url: '/update',"
  "type: 'POST',"
  "data: data,"
  "contentType: false,"
  "processData:false,"
  "xhr: function() {"
  "var xhr = new window.XMLHttpRequest();"
  "xhr.upload.addEventListener('progress', function(evt) {"
  "if (evt.lengthComputable) {"
  "var per = evt.loaded / evt.total;"
  "$('#prg').html('Progresso: ' + Math.round(per*100) + '%');"
  "}"
  "}, false);"
  "return xhr;"
  "},"
  "success:function(d, s) {"
  "console.log('Sucesso!')"
 "},"
 "error: function (a, b, c) {"
 "}"
 "});"
 "});"
 "</script>";
