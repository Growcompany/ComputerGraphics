# Computer Graphics Assignments

## Overview
이 프로젝트는 평면과 세 개의 구로 구성된 장면을 소프트웨어 기반의 레이 트레이싱 알고리즘으로 렌더링하는 예제입니다.  
실제 레이 트레이싱 및 음영 계산은 직접 구현하며, OpenGL/GLUT(또는 OpenCV)는 최종 결과 이미지를 화면에 출력하는 데에만 사용됩니다.

---

## Table of Contents
- [Compilation Instructions](#compilation-instructions)
- [Run Instructions](#run-instructions)
- [Short Description](#short-description)

---

## Compilation Instructions

### Using Visual Studio
1. **솔루션 열기**  
   `openglviewer.sln` 파일을 Visual Studio에서 엽니다.
2. **프로젝트 설정 확인**  
   - **Include 디렉토리:** `include` 폴더 경로가 포함되어야 합니다.  
   - **Library 디렉토리:** `lib` 폴더 경로가 포함되어야 합니다.
3. **빌드 구성 선택**  
   Debug 또는 Release 모드를 선택합니다.
4. **Clean & Rebuild**  
   상단 메뉴에서 **Build > Clean Solution**을 선택한 후, **Build > Rebuild Solution**을 실행하여 최신 코드를 빌드합니다.

---

## Run Instructions
1. 빌드가 완료되면, 생성된 실행 파일(예: `OpenGLViewer.exe`)이 `bin` 또는 `emptyviewer/Debug` 폴더에 생성됩니다.
2. 해당 실행 파일을 실행합니다.
3. 프로그램 실행 후 창이 열리며, 소프트웨어로 계산한 최종 이미지가 화면에 출력됩니다.
4. 프로그램 종료는 **ESC** 키 또는 **Q** 키를 누르면 됩니다.

---

## Short Description

### 메인 함수 (`main`)
- 프로그램의 시작점으로, 윈도우를 생성하고 이벤트 루프를 관리하며, 최종적으로 렌더링된 이미지를 화면에 출력합니다.

### 렌더 함수 (`render`, `render2`, `render3` 등)
- 소프트웨어 레이 트레이싱 알고리즘을 구현하여, 각 픽셀마다 광선을 생성합니다.
- 장면의 객체(평면, 구 등)와의 교차 여부를 검사한 후, 그 결과에 따라 픽셀의 색상을 결정합니다.

### 카메라 및 광선 생성
- 카메라의 위치, 시야 영역, 이미지 평면까지의 거리 등을 설정합니다.
- 각 픽셀에 대해 카메라에서 광선을 생성하는 역할을 합니다.

### 교차 검사 함수 (예: `intersectSphere`, `trace`)
- 생성된 광선과 장면의 객체 간의 교차를 검사합니다.
- 객체와의 충돌 여부와 교차점을 결정합니다.
