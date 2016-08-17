---------------------------
 Attention
---------------------------
- After uploading this game sketch, you may not be able to uploading next game sketch. If you get the error message below, please use the recover method below to upload the next sketch:
  - Error message:
    - "Couldn't find a Board on the selected port. Check that you have the correct port selected.  If it is correct, try pressing the board's reset button after initiating the upload."
  - Recover method (PORTS-Reset Method):
    (1) Please set: ArduinoIDE > File > Preferences > Settings > Show verbose output during: upload = checked
    (2) Keep to press a reset button of Arduboy at the side of the USB cable.
    (3) Push upload button of Arduino IDE.
    (4) Release the reset button of Arduboy when a message "PORTS {} / {} => {}" is output continuously.
---------------------------
 ご注意
---------------------------
- このゲームのスケッチを転送した後に次のゲームのスケッチをアップロードできなくなるかもしれません。もし下記のようなエラーメッセージがでたら、下記の修復方法で修復し、次のスケッチを転送してください。
  - エラーメッセージ:
    - "Couldn't find a Board on the selected port. Check that you have the correct port selected.  If it is correct, try pressing the board's reset button after initiating the upload."
  - 修復方法(PORTS-リセット法):
    (1) ArduinoIDE > ファイル > 環境設定> 設定 > より詳細な情報を表示する: 書き込み = チェック有にします。
    (2) Arduboy のUSBケーブルの横のリセットボタンを押したままにする。(爪楊枝の先を爪切りで1mm切ったものを使うと便利です)
    (3) Arduino IDE の書き込みボタンを押します。
    (4) Arduino IDE に "PORTS {} / {} => {}" というメッセージが連続して出力される状態の時に Arduboy のリセットボタンを離します。 

---------------------------
Tested Environment
---------------------------
- Arduboy 1.0
- Arduboy Library:
  - https://github.com/Arduboy/Arduboy/
  - revision: 3c409fefb
  - branch  : master
- Arduino IDE 1.6.9
---------------------------
Description  
---------------------------
- This is a shooter game !
- Shoot circle enemies.
- The line at bottom is your hit point.
- The line at above that is enemy's hit point.
- Key:
  - UDLR ... move
  - B    ... shot
  - A    ... none
--------------------------------
EOF