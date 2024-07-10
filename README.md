[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/omXkVCQu)
[![Open in Visual Studio Code](https://classroom.github.com/assets/open-in-vscode-2e0aaae1b6195c2367325f4f02e2d04e9abb55f0b24a779b69b11b9e10269abc.svg)](https://classroom.github.com/online_ide?assignment_repo_id=14003738&assignment_repo_type=AssignmentRepo)
![header](https://capsule-render.vercel.app/api?type=rounded&height=300&color=gradient&text=SAILS%20TOGEHTER&fontSize=50&fontAlignY=50)

<br> 

# 2024 캡스톤 20조

**Unreal Engine 5를 활용한 멀티플레이어 협동 게임 제작**


# 프로젝트 소개
**[Unreal Engine 5를 활용한 멀티플레이어 게임 제작]**
<br>
'Sails Together'는 협동 어드벤처 게임으로, 플레이어들은 최대 4인까지 팀을 구성하여 모험을 떠난다. <br> 배를 업그레이드 시키며 보물을 찾아내는 것이 목적이다. <br> 플레이어들은 이 과정에서 협동하여 장애물을 파괴하고, 적들을 물리치며 스테이지를 진행해나간다.
<br>
프로젝트는 Unreal을 기반으로 제작되었으며, 멀티플레이어 플랫폼인 Steam 아이디만 있다면 먼거리에서도 자유로운 멀티플레이가 가능하며 로컬에서도 멀티플레이가 가능하다.

**적용 기능**
1. 차세대 물리 엔진 '카오스'를 활용하여 바람에 따라 흔들리는 돛과 오브젝트 파괴 시뮬레이션 구현
2. '캐스케이드 파티클 시스템'을 이용한 생동감 있는 이펙트 구현
3. 'Navigation Mesh'을 적용하여 동적으로 변화하는 지형에 따른 길 찾기 구현
4. 별도의 서버를 구축할 필요 없이 특정 Client가 서버 역할을하는 'Listen Server' 방식 채택
5. 클라이언트에서 직접 이동 계산, 서버는 패킷 검증만 하여 최적화 및 치트 방지
6. '애니메이션 블렌딩'을 통해 부드러운 애니메이션 전환 구현

<br>

# [🔗 소개 영상 🔗](https://youtu.be/SULgr9ooJ6w)
[![Video Label](http://img.youtube.com/vi/SULgr9ooJ6w/0.jpg)](https://youtu.be/SULgr9ooJ6w)

<br>

# 😄  팀 소개


#### 강현서

~~~
Student ID : ****1546
E-mail: hansx2079@kookmin.ac.kr
~~~

#### 나민형

~~~
Student ID : ****1583
E-mail: minhyoung0724@kookmin.ac.kr
~~~

#### 신무현

~~~
Student ID : ****1613
E-mail: alsk1976@kookmin.ac.kr
~~~

#### 최태호

~~~
Student ID : ****1683
E-mail: sotetero11@gmail.com
~~~

<br>

#  기술 스택

#### Game Development

<img src="https://img.shields.io/badge/C++-00599C?style=for-the-badge&logo=cplusplus&logoColor=white">
<br>
<img src="https://img.shields.io/badge/unrealengine-%23313131.svg?style=for-the-badge&logo=unrealengine&logoColor=white">
<br>
<img src="https://img.shields.io/badge/blender-E87D0D?style=for-the-badge&logo=blender&logoColor=white">

<br>

# 포스터

<img width=100% src="https://github.com/kookmin-sw/capstone-2024-20/assets/54923850/02cdb97f-7734-40eb-b351-7c2d44d1324c">


# 중간발표 관련 자료 다운로드

[중간발표 프레젠테이션](https://drive.google.com/file/d/1e_3hu5goN5XIxOw2ZKw2Q_bnr2bv6gvb/view?usp=sharing)

[(별첨) 중간보고서](https://drive.google.com/file/d/1zfASZNG8n7h_XWEoO57KzcTC41XICkVe/view?usp=sharing)

[(별첨) 시연 동영상](https://youtu.be/nFXJS2uptlU)

<br>

# 최종발표 관련 자료 다운로드
[최종발표 프레젠테이션](https://drive.google.com/file/d/1rRtya80SZuzJDb9gb48VrIGc1ilAUDj2/view?usp=sharing)

[(별첨) 최종보고서](https://drive.google.com/file/d/1UQ98HsjDjUy3ccjrAhI1NjxiGGjzCyrT/view?usp=sharing)

[(별첨) 시연 동영상](https://youtu.be/SULgr9ooJ6w)


# 🎮 사용법

## 릴리즈 다운로드 (권장)

최신 릴리즈 버전 : 0.2.3

1. [Google Drive](https://drive.google.com/drive/folders/1x4PCA_ipOcgtplP-wAv5WBOhIzESkYWc?usp=sharing) 에서 원하는 버전의 릴리즈를 다운로드 합니다.

2. 다운로드한 ZIP 파일을 압축 해제 합니다.

3. 압축 해제한 폴더 내부의 **Windows/capstone_2024_20.exe** 파일을 실행합니다.

## 빌드

1. [Download Unreal Engine](https://www.unrealengine.com/en-US/download) 의 설명에 따라, Epic Games Launcher를 설치합니다.

2. Epic Games Launcher를 실행한 후, **Unreal Engine 5.3.2** 를 설치합니다.

3. 본 Repository의 master Branch를 Clone 합니다.

4. **capstone_2024_20/capstone_2024_20.uproject** 파일을 우클릭하여, 'Create Visual Studio Solution' 을 선택합니다.

6. 생성된 .sln 파일을 실행합니다.

7. **Games/capstone_2024_20** 프로젝트를 우클릭하여, 'Rebuild' 를 선택합니다.
