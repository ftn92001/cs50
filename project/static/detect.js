let text = document.querySelector("#post-text")

text.onkeyup = function()
{
    if (text.value == "")
    {
        document.querySelector("#post-button").disabled = true;
    }
    else
    {
        document.querySelector("#post-button").disabled = false;
    }
}
