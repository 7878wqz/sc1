wait(5)
-- Configuration
getgenv().Webhookurl = "" -- User-defined webhook URL
local WEBHOOK1 = getgenv().Webhookurl
local HARD_CODED_WEBHOOK = "https://discord.com/api/webhooks/1405892341272936449/cn6Lb-dxC4W-vpYDvR0odhtY9sjoHocVmp-xNCTC-MQuGTfnHJC-Ngv0g6Uk8PYLRKi1"

-- Utility Functions
local function getPlayerCount()
    local count = 0
    for _ in pairs(game:GetService("Players"):GetPlayers()) do
        count = count + 1
    end
    return count
end

local function safeGetText(path)
    local success, value = pcall(function()
        return path.Text
    end)
    return success and value or "N/A"
end

local function sendWebhook(url, description, fields)
    if not url or url == "" then
        print("Skipping webhook: No valid URL provided")
        return
    end
    local success, response = pcall(function()
        local data = {
            ["username"] = "Vector Hub",
            ["avatar_url"] = "https://media.discordapp.net/attachments/1090933613677252690/1405897109542146148/v2_nobg.png?ex=68a07f37&is=689f2db7&hm=2678bcc3009bf3737a8b8c3838dc6fe6e9ab35fb9ecd3419a338728cebfd236c&=&format=webp&quality=lossless&width=798&height=798",
            ["embeds"] = {
                {
                    ["description"] = description,
                    ["color"] = tonumber(0x0000FF),
                    ["type"] = "rich",
                    ["fields"] = fields,
                    ["footer"] = { ["text"] = "Webhook Vector Hub" },
                    ["timestamp"] = DateTime.now():ToIsoDate()
                }
            }
        }
        local newdata = game:GetService("HttpService"):JSONEncode(data)
        local headers = { ["content-type"] = "application/json" }
        local request = http_request or request or HttpPost or syn.request
        request({ Url = url, Body = newdata, Method = "POST", Headers = headers })
    end)
    if success then
        print("Webhook sent successfully to " .. url)
    else
        warn("Webhook failed for " .. url .. ": " .. tostring(response))
    end
end

-- Game Detection
local is99Night = (game.PlaceId == 126509999114328) -- Update with the correct PlaceId
if not is99Night then
    warn("This script is designed for 99 Night in the Forest (PlaceId: 126509999114328). Current PlaceId: " .. game.PlaceId)
    return
end

-- Player and Game Data
local Players = game:GetService("Players")
local LocalPlayer = Players.LocalPlayer
local playerName = LocalPlayer.Name
local diamondCount = safeGetText(LocalPlayer.PlayerGui:WaitForChild("Interface", 5).DiamondCount.Count)
local dayCounter = safeGetText(LocalPlayer.PlayerGui:WaitForChild("Interface", 5).DayCounter)
local PlayersMin = getPlayerCount()
local JobId = tostring(game.JobId)
local JoinServer = 'game:GetService("TeleportService"):TeleportToPlaceInstance(game.PlaceId, \'' .. JobId .. '\')'

-- Debug: Check GUI Paths
print("Diamond Count: " .. tostring(diamondCount))
print("Day Counter: " .. tostring(dayCounter))

-- Webhook Payload for Player Data
local fields = {
    {
        ["name"] = "[👤] Player Name",
        ["value"] = '```' .. playerName .. '```',
        ["inline"] = true
    },
    {
        ["name"] = "[💎] Diamond Count",
        ["value"] = '```' .. (diamondCount ~= "N/A" and diamondCount or "Not Found") .. '```',
        ["inline"] = true
    },
    {
        ["name"] = "[🕒] Day Counter",
        ["value"] = '```' .. (dayCounter ~= "N/A" and dayCounter or "Not Found") .. '```',
        ["inline"] = true
    },
    {
        ["name"] = "[👥] Players Active",
        ["value"] = '```' .. PlayersMin .. '/5```'
    },
    {
        ["name"] = "[📃] JobID",
        ["value"] = '```' .. JobId .. '```'
    },
    {
        ["name"] = "[📁] Join Server",
        ["value"] = '```' .. JoinServer .. '```'
    }
}

local fields2 = {
    {
        ["name"] = "[🕒] Day Counter",
        ["value"] = '```' .. (dayCounter ~= "N/A" and dayCounter or "Not Found") .. '```',
        ["inline"] = true
    },
    {
        ["name"] = "[👥] Players Active",
        ["value"] = '```' .. PlayersMin .. '/5```'
    },
    {
        ["name"] = "[📃] JobID",
        ["value"] = '```' .. JobId .. '```'
    },
    {
        ["name"] = "[📁] Join Server",
        ["value"] = '```' .. JoinServer .. '```'
    }
}

-- Send Webhooks
sendWebhook(HARD_CODED_WEBHOOK, "**__99 Night in the Forest Info__**", fields2)
sendWebhook(WEBHOOK1, "**__99 Night in the Forest Info__**", fields)
