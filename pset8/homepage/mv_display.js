document.querySelector(".a").onclick = function()
{
    let mv = document.querySelector('#a');
    if (mv.style.display === 'none' || mv.style.display === '')
    {
        mv.style.display = 'inline';
    }
    else
    {
        mv.style.display = 'none';
    }

}

document.querySelector(".b").onclick = function()
{
    let mv = document.querySelector('#b');
    if (mv.style.display === 'none' || mv.style.display === '')
    {
        mv.style.display = 'inline';
    }
    else
    {
        mv.style.display = 'none';
    }
}