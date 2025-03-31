# Define sources
target_sources(${PROJECT_NAME} PRIVATE

        ${SOURCE_DIR}/Game.cpp
        ${SOURCE_DIR}/Game.h

        ${SOURCE_DIR}/Input/InputCodes.h
        ${SOURCE_DIR}/Input/Input.cpp
        ${SOURCE_DIR}/Input/Input.h

        ${SOURCE_DIR}/Visuals/AnimatedSprite.cpp
        ${SOURCE_DIR}/Visuals/AnimatedSprite.h
)
