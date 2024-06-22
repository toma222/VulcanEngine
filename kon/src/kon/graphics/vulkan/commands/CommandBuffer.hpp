
#pragma once

namespace kon
{
    class CommandBuffer
    {
    public:
        CommandBuffer();
        ~CommandBuffer();

        void StartSingleTimeCommands();
        void EndSingleTimeCommands();

    private:
        
    };
}
