#!/system/bin/sh
       if [ ! -f /sdcard/Apple/gamelist.txt ]; then
	       echo "Wrong directory"
        fi
   shell() {
     cont="$1"
     cmd notification post -I /sdcard/Apple/Priority_logo.png -S bigtext -t '♨️ I/O Priority ' 'Tag' "$cont" > /dev/null 2>&1 
   }
  echo 
  echo 
  echo "    (+)================================(+)"
  echo
  echo "        Declarasi Projek I/O priorty...."
  echo 
  echo "        Author                :  @Yeye_nat"
  echo "        Github                :  Yeye-PID"
  echo "        Release              :  11 - janu - 2025"
  echo "        Script name       :  I/O priorty "
  echo "     _____________________________________"
  echo "    \____________________________________/"
  echo
  echo
  echo "   I/O priorty wait to game V1.0"
  sleep 2
  echo
  echo
        # Ambil daftar paket
      package_list=$(pm list package | cut -f2 -d: | tr -d '\r' | xargs -n1)
     control=1
       while IFS= read -r gamelist || [ -n "$gamelist" ]; do
          line=$(echo "$gamelist" | tr -d '\r' | xargs)
              if [ -n "$line" ]; then
        if echo "$package_list" | grep -xq "$line"; then
            echo "  $control. $line"
            control=$((control + 1))
          else
            echo "Paket game '$line' tidak ditemukan."
              fi
                fi
            done < /sdcard/Apple/gamelist.txt
          # Buat varibale instlangi toast.apk
          if [ -f /sdcard/Apple/toast.apk ]; then
    if pm list package | cut -f2 -d: | grep bellavita.toast; then
           echo
     else       
         cp /sdcard/Apple/toast.apk /data/local/tmp/
          pm install /data/local/tmp/toast.apk
       fi
     fi > /dev/null 2>&1       
     # Buat varibale instlling and uninstalling script
  if [ "$1" = kill ]; then
        if pgrep -f service > /dev/null 2>&1; then
         echo "  Program is stopped in the backgurond "
         pm uninstall bellavita.toast > /dev/null 2>&1
         rm /data/local/tmp/service > /dev/null 2>&1
         pkill -f service > /dev/null 2>&1
         shell "Program is stopped in the backgurond" 
         pkill -f sh > /dev/null 2>&1       
     else
       echo "Porgam faild stop !"
   fi
  else
     if pgrep -f service > /dev/null 2>&1; then
       cp /sdcard/Apple/service /data/local/tmp
       chmod +x /data/local/tmp/service
      nohup /data/local/tmp/service > /dev/null 2>&1
    fi 
      sleep 2
        if pgrep -f service > /dev/null 2>&1; then
        echo " Program is running in the backgurond"
     else
          echo "Porgram faild running !"
     fi
  fi 