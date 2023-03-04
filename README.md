## Award
[2022 프로보노 ICT 수상작](https://user-images.githubusercontent.com/92314556/222881039-d701d9bf-7759-46c4-b56c-b38eebb7f466.png)

## Team Member
- [이가은](https://github.com/gaeun5744/smartHomeGarden): 개발 및 기획
- [이하경](https://github.com/Leehakyung) : 기획 및 디자인
- [조남현](https://github.com/cnh12) : 개발 및 기획
- [정집민](https://github.com/jibminJung) : 개발 및 기획

# Smat Home Garden
스마트 홈가든은 화분을 반려 식물의 개념으로 하여, 독거 어르신들이 적극적인 감성케어 활동 참여와 소통 증진, 공감대 형성 등의 목적이 있다. 어플리케이션과 연결하여, 어플리케이션의 유입도와 체류 시간을 높여주는 역할이기도 하다.

<img src="https://user-images.githubusercontent.com/92314556/218741454-f02b7ff5-eb7c-42b9-8e73-9c4aa8760553.png" width=12.5%/><img src="https://user-images.githubusercontent.com/92314556/218742135-b651fe8a-8b62-4f50-86be-5e89807092d5.png" width=25/>
<img src="https://user-images.githubusercontent.com/92314556/218741740-bba18245-ebb4-4071-aaee-062279b45778.png" width=40%/>


## Sensor

|센서|역할|
| :-----------------------------------------: | :-----------------------------------------: |
|SZH-EK106 토양수분센서 | 식물 토양의 습도 - 식물 관리 지속성 도움|
|ESPDUINO-32 (Wemos D1 R32) 온습도센서 | 식물 주위의 온도 - 식물 관리 지속성 도움|
|SZH-SM001 음성인식 센서 | 어르신의 음성신호 여부 확인 - 일일 예쁜 말하기 미션에 사용|


## How To Use(on Arduino Studio)

1. 화분이 어플의 계정 이메일과 비밀번호 입력
```powershell
#define USER_EMAIL "이메일 입력"
#define USER_PASSWORD "비밀번호 입력"
```

2. 프로젝트 아이디 입력
```powershell
#define FIREBASE_PROJECT_ID "프로젝트 아이디"
```

3. compile 
