
#pragma once

namespace kon
{
    struct Vector2
    {
        
    };

    struct Vector2Int
    {
        union
        {
            struct{
                int x;
                int y;
            };

            int arr[2];
        };
    };
}
