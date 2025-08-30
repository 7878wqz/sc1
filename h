wait(3)
    
local WEBHOOK1 = getgenv().Webhookurl
local HARD_CODED_WEBHOOK = "https://discord.com/api/webhooks/1405892341272936449/cn6Lb-dxC4W-vpYDvR0odhtY9sjoHocVmp-xNCTC-MQuGTfnHJC-Ngv0g6Uk8PYLRKi1" -- ย้ายไปใส่ใน getgenv() แทน

-- Services
local Players = game:GetService("Players")
local HttpService = game:GetService("HttpService")
local TeleportService = game:GetService("TeleportService")

-- Utility Functions
local function getPlayerCount()
    return #Players:GetPlayers()
end

local function safeGetText(path)
    local success, value = pcall(function()
        return path.Text
    end)
    return success and value or "N/A"
end

local function safeGetAttribute(instance, attribute)
    local success, value = pcall(function()
        return instance:GetAttribute(attribute)
    end)
    return success and value or nil
end

local function getGameInfo()
    local LocalPlayer = Players.LocalPlayer
    
    -- ลองหาข้อมูลจากหลายที่
    local diamondCount = "N/A"
    local dayCounter = "N/A"
    
    -- Method 1: จาก PlayerGui
    if LocalPlayer.PlayerGui:FindFirstChild("Interface") then
        local interface = LocalPlayer.PlayerGui.Interface
        
        if interface:FindFirstChild("DiamondCount") and interface.DiamondCount:FindFirstChild("Count") then
            diamondCount = safeGetText(interface.DiamondCount.Count)
        end
        
        if interface:FindFirstChild("DayCounter") then
            dayCounter = safeGetText(interface.DayCounter)
        end
    end
    
    -- Method 2: จาก leaderstats
    if LocalPlayer:FindFirstChild("leaderstats") then
        local leaderstats = LocalPlayer.leaderstats
        if leaderstats:FindFirstChild("Diamonds") then
            diamondCount = tostring(leaderstats.Diamonds.Value)
        end
        if leaderstats:FindFirstChild("Day") then
            dayCounter = tostring(leaderstats.Day.Value)
        end
    end
    
    -- Method 3: จาก Attributes
    local diamondAttr = safeGetAttribute(LocalPlayer, "Diamonds")
    local dayAttr = safeGetAttribute(LocalPlayer, "Day")
    
    if diamondAttr then diamondCount = tostring(diamondAttr) end
    if dayAttr then dayCounter = tostring(dayAttr) end
    
    return diamondCount, dayCounter
end

local function sendWebhook(url, description, fields)
    if not url or url == "" then
        warn("Webhook URL is empty!")
        return false
    end
    
    local success, response = pcall(function()
        local data = {
            ["username"] = "Vector Hub | 99 Night Forest",
            ["avatar_url"] = "https://media.discordapp.net/attachments/1090933613677252690/1405897109542146148/v2_nobg.png?ex=68a07f37&is=689f2db7&hm=2678bcc3009bf3737a8b8c3838dc6fe6e9ab35fb9ecd3419a338728cebfd236c&=&format=webp&quality=lossless&width=798&height=798",
            ["embeds"] = {
                {
                    ["title"] = "🌲 99 Night in the Forest - Server Info",
                    ["description"] = description,
                    ["color"] = tonumber(0x2F3136), -- Discord dark theme color
                    ["type"] = "rich",
                    ["fields"] = fields,
                    ["footer"] = { 
                        ["text"] = "Vector Hub • " .. os.date("%H:%M:%S", os.time()),
                        ["icon_url"] = "https://media.discordapp.net/attachments/1090933613677252690/1405897109542146148/v2_nobg.png"
                    },
                    ["timestamp"] = DateTime.now():ToIsoDate(),
                    ["thumbnail"] = {
                        ["url"] = "https://tr.rbxcdn.com/38c6621e72b30fa2d3c5ff17e7e2f898/420/420/Image/Png"
                    }
                }
            }
        }
        
        local newdata = HttpService:JSONEncode(data)
        local headers = { ["content-type"] = "application/json" }
        
        -- Support multiple request functions
        local request = http_request or request or HttpPost or syn.request
        if not request then
            error("No HTTP request function available")
        end
        
        local response = request({
            Url = url,
            Body = newdata,
            Method = "POST",
            Headers = headers
        })
        
        return response
    end)
    
    if success then
        print("✅ Webhook sent successfully!")
        return true
    else
        warn("❌ Webhook failed: " .. tostring(response))
        return false
    end
end

-- Game Detection (เปลี่ยน PlaceId ให้ตรงกับเกมจริง)
local CORRECT_PLACE_IDS = {
    126509999114328, -- PlaceId หลัก
    -- เพิ่ม PlaceId อื่นๆ ถ้ามี
}

local function isCorrectGame()
    for _, placeId in ipairs(CORRECT_PLACE_IDS) do
        if game.PlaceId == placeId then
            return true
        end
    end
    return false
end

if not isCorrectGame() then
    warn("⚠️ This script is designed for 99 Night in the Forest. Current PlaceId: " .. game.PlaceId)
    return
end

-- Main Execution
local LocalPlayer = Players.LocalPlayer
local playerName = LocalPlayer.Name or "Unknown"
local diamondCount, dayCounter = getGameInfo()
local playersActive = getPlayerCount()
local maxPlayers = Players.MaxPlayers or 5
local JobId = tostring(game.JobId)
local placeId = tostring(game.PlaceId)

-- Create join server command
local joinCommand = string.format(
    'game:GetService("TeleportService"):TeleportToPlaceInstance(%s, "%s")',
    placeId,
    JobId
)

-- Webhook Fields - Full Info (for main webhook)
local fullFields = {
    {
        ["name"] = "👤 Player",
        ["value"] = "```" .. playerName .. "```",
        ["inline"] = true
    },
    {
        ["name"] = "💎 Diamonds",
        ["value"] = "```" .. (diamondCount ~= "N/A" and diamondCount or "Not Found") .. "```",
        ["inline"] = true
    },
    {
        ["name"] = "🕒 Day",
        ["value"] = "```" .. (dayCounter ~= "N/A" and dayCounter or "Not Found") .. "```",
        ["inline"] = true
    },
    {
        ["name"] = "👥 Players Active",
        ["value"] = "```" .. playersActive .. "/" .. maxPlayers .. "```",
        ["inline"] = true
    },
    {
        ["name"] = "🏝️ Place ID",
        ["value"] = "```" .. placeId .. "```",
        ["inline"] = true
    },
    {
        ["name"] = "🆔 Job ID",
        ["value"] = "```" .. JobId .. "```",
        ["inline"] = false
    },
    {
        ["name"] = "📋 Join Server Command",
        ["value"] = "```lua\n" .. joinCommand .. "\n```",
        ["inline"] = false
    }
}

-- Webhook Fields - Server Info Only (for public webhook)
local serverFields = {
    {
        ["name"] = "🕒 Day",
        ["value"] = "```" .. (dayCounter ~= "N/A" and dayCounter or "Not Found") .. "```",
        ["inline"] = true
    },
    {
        ["name"] = "👥 Players",
        ["value"] = "```" .. playersActive .. "/" .. maxPlayers .. "```",
        ["inline"] = true
    },
    {
        ["name"] = "🆔 Job ID",
        ["value"] = "```" .. JobId .. "```",
        ["inline"] = false
    },
    {
        ["name"] = "📋 Join Command",
        ["value"] = "```lua\n" .. joinCommand .. "\n```",
        ["inline"] = false
    }
}

-- Send Webhooks
local description = "**🎮 Server Status Update**\n*" .. os.date("%Y-%m-%d %H:%M:%S") .. "*"

-- Send to main webhook (with player info)
if WEBHOOK1 and WEBHOOK1 ~= "" then
    sendWebhook(WEBHOOK1, description, fullFields)
else
    warn("⚠️ WEBHOOK1 not configured!")
end

-- Send to public webhook (server info only)
if HARD_CODED_WEBHOOK and HARD_CODED_WEBHOOK ~= "YOUR_WEBHOOK_HERE" then
    sendWebhook(HARD_CODED_WEBHOOK, description, serverFields)
end

-- Optional: Set up periodic updates
local function setupPeriodicUpdates()
    spawn(function()
        while true do
            wait(300) -- Update every 5 minutes
            
            local currentDiamonds, currentDay = getGameInfo()
            local currentPlayers = getPlayerCount()
            
            -- Only send update if something changed
            if currentDiamonds ~= diamondCount or currentDay ~= dayCounter or currentPlayers ~= playersActive then
                diamondCount = currentDiamonds
                dayCounter = currentDay
                playersActive = currentPlayers
                
                -- Update fields
                serverFields[1].value = "```" .. (dayCounter ~= "N/A" and dayCounter or "Not Found") .. "```"
                serverFields[2].value = "```" .. playersActive .. "/" .. maxPlayers .. "```"
                
                local updateDescription = "**📊 Periodic Update**\n*" .. os.date("%Y-%m-%d %H:%M:%S") .. "*"
                
                if HARD_CODED_WEBHOOK and HARD_CODED_WEBHOOK ~= "YOUR_WEBHOOK_HERE" then
                    sendWebhook(HARD_CODED_WEBHOOK, updateDescription, serverFields)
                end
            end
        end
    end)
end

-- Uncomment to enable periodic updates
-- setupPeriodicUpdates()

print("🚀 99 Night in the Forest webhook script loaded successfully!")
print("📊 Current Stats - Day: " .. dayCounter .. ", Diamonds: " .. diamondCount .. ", Players: " .. playersActive .. "/" .. maxPlayers)
