#pragma once
// Project specific
// Standard Libraries
#include <unordered_map>
#include <vector>
#include <DirectXMath.h>


namespace DoremiEngine
{
    namespace Core
    {
        class SharedContext;
    }
}

namespace Doremi
{
    namespace Core
    {

        class InputHandler;
        class AudioHandler
        {
            public:
            /** Is a singleton. Use this method to get the EventManager*/
            static AudioHandler* GetInstance();
            static void StartAudioHandler(const DoremiEngine::Core::SharedContext& p_sharedContext);
            static void StopAudioHandler();
            AudioHandler(const DoremiEngine::Core::SharedContext& p_sharedContext);
            ~AudioHandler();
            void Initialize();
            void SetGunButtonDownTime(double p_time);
            void StartContinuousRecording();
            void StartRepeatableRecording();
            void SetupContinuousRecording();
            void SetupRepeatableRecording();
            void PlayRepeatableRecordedSound();
            void Update(double p_deltaTime);
            float GetFrequency() const { return m_currentFrequency; };
            float GetRepeatableSoundFrequency();

            private:
            const DoremiEngine::Core::SharedContext& m_sharedContext;

            InputHandler* m_inputHandler;

            double m_accumulatedDeltaTime;
            std::vector<float>m_frequencies;

            float m_frequencyVectorPrecision;
            static AudioHandler* m_singleton;
            size_t m_continuousFrequencyAnalyserChannelID;
            size_t m_continuousFrequencyAnalyserSoundID;
            size_t m_repeatableFrequencyAnalyserChannelID;
            size_t m_repeatableFrequencyAnalyserSoundID;
            size_t m_outputRepeatableSoundChannelID;
            size_t m_outputRepeatableSoundID;
            float m_currentFrequency;
            bool m_repeatableAnalysisComplete;
            bool m_analyseActive;
            bool m_repeatableAnalysisActive;
            bool m_continuousRecording;
            double m_timeGunReloadButtonWasPressed;


            enum SoundState
            {
                ANALYSECONTINUOUS,
                HOLDCONTINUOUSANALYSIS,
                ANALYSEREPEATABLE,
                HOLDREPEATABLEANALYSIS,

            };
            SoundState m_SoundState;
        };
    }
}
