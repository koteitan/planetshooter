----------------------
 planetshooter
----------------------
- English
  - How to load
  - Attention
  - How to Play
  - Tested Environment
  
- ���{��
  - ���[�h�̎d��
  - ������
  - �V�ѕ�
  - �e�X�g�ς݊�

---------------------------
How to load
---------------------------
- Install Arduino IDE
  - https://www.arduino.cc/en/Main/Software
- Set the preferences below of Arduino IDE:
  - File > Preferences > Settings Tab > Additional Boards Manager URLs = 
    https://arduboy.github.io/board-support/package_arduboy_index.json
  - Tool > Board       = Arduboy
  - Tool > Serial Port = the port of Arduboy Leonardo
- Get the sketch
  - git clone https://github.com/koteitan/planetshooter.git
    or
  - https://github.com/koteitan/planetshooter/archive/master.zip
- change the directory name to "planetshooter/".
- Connect USB cable between your pc and Arduboy.
- Press upload button.
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
How to Play
---------------------------
- This is a shooter game !
- Shoot circle enemies.
- The line at bottom is your hit point.
- The line at above that is enemy's hit point.
- Key:
  - UDLR ... move
  - B    ... shot
  - A    ... none
---------------------------
Tested Environment
---------------------------
- Arduboy 1.0
- Arduboy Library:
  - https://github.com/Arduboy/Arduboy/
  - revision: 3c409fefb
  - branch  : master
- Arduino IDE 1.6.10

---------------------------
���[�h�̎d��
---------------------------
- Arduino IDE ���C���X�g�[�����Ă�������:
  - https://www.arduino.cc/en/Main/Software
- ���L��Arduino IDE �̐ݒ�����ĉ�����:
  - �t�@�C�� > ���ݒ� > �ǉ��̃{�[�h�}�l�[�W����URL = 
    https://arduboy.github.io/board-support/package_arduboy_index.json
  - �c�[�� > �{�[�h         = Arduboy
  - �c�[�� > �V���A���|�[�g = Arduboy Leonardo ������|�[�g
- �X�P�b�`���_�E�����[�h���Ă�������:
  - git clone https://github.com/koteitan/planetshooter.git
    �܂���
  - https://github.com/koteitan/planetshooter/archive/master.zip
- �f�B���N�g������ "planetshooter/" �ɕς��ĉ������B
- Arduboy �� USB �P�[�u�� �� PC �ɂȂ��ŉ������B
- �u�}�C�R���{�[�h�ɏ������ށv�{�^���������ĉ������B
---------------------------
 ������
---------------------------
- ���̃Q�[���̃X�P�b�`��]��������Ɏ��̃Q�[���̃X�P�b�`���A�b�v���[�h�ł��Ȃ��Ȃ邩������܂���B�������L�̂悤�ȃG���[���b�Z�[�W���ł���A���L�̏C�����@�ŏC�����A���̃X�P�b�`��]�����Ă��������B
  - �G���[���b�Z�[�W:
    - "Couldn't find a Board on the selected port. Check that you have the correct port selected.  If it is correct, try pressing the board's reset button after initiating the upload."
  - �C�����@(PORTS-���Z�b�g�@):
    (1) ArduinoIDE > �t�@�C�� > ���ݒ�> �ݒ� > ���ڍׂȏ���\������: �������� = �`�F�b�N�L�ɂ��܂��B
    (2) Arduboy ��USB�P�[�u���̉��̃��Z�b�g�{�^�����������܂܂ɂ���B(�ܗk�}�̐��ܐ؂��1mm�؂������̂��g���ƕ֗��ł�)
    (3) Arduino IDE �̏������݃{�^���������܂��B
    (4) Arduino IDE �� "PORTS {} / {} => {}" �Ƃ������b�Z�[�W���A�����ďo�͂�����Ԃ̎��� Arduboy �̃��Z�b�g�{�^���𗣂��܂��B 
---------------------------
�V�ѕ�  
---------------------------
- �V���[�e�B���O�Q�[���ł��B!
- �ۂ��G�������ĉ������B
- ���̐��͂��Ȃ��̃q�b�g�|�C���g�ł��B
- ���̏�̐��͓G�̃q�b�g�|�C���g�ł��B
- ���ɏI���͂���܂���B
- �L�[:
  - UDLR ... �ړ�
  - B    ... �V���b�g
  - A    ... �Ȃ�
--------------------------------
---------------------------
�e�X�g�ς݊�
---------------------------
- Arduboy 1.0
- Arduboy Library:
  - https://github.com/Arduboy/Arduboy/
  - revision: 3c409fefb
  - branch  : master
- Arduino IDE 1.6.10
EOF
