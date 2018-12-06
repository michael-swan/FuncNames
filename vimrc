set noswapfile
set nowrap

set laststatus=2

let b:func_names = {}
function! UpdateFuncName()
	let func_name_out = system('~/.vim/func-name ' . expand('%:p'))
	let b:func_names = {}
	for line in split(func_name_out, "\n")
		let line_parts = split(line, " | ")
		if len(line_parts) == 3
			let fn = line_parts[0]
			let beg = split(line_parts[1], ":")
			let end = split(line_parts[2], ":")
			if beg[0] == expand('%:p')
				let b:func_names[fn] = { 'beg': { 'line': beg[1], 'char': beg[2] }, 'end': { 'line': end[1], 'char': end[2] } }
			endif
		endif
	endfor
endfunction

autocmd BufNewFile,BufRead *.c call UpdateFuncName()

function! StatuslineFunc()
	if !exists('b:func_names')
		return '?'
	endif
	let pos = getcurpos()
	let row = pos[1]
	let col = pos[2]
	for [k,v] in items(b:func_names)
	  if v.beg.line <= row && v.end.line >= row
	    return k
	  endif
	endfor
	return '?'
endfunction

function! GoToFunc(gotoBeg)
	if !exists('b:func_names')
		return
	endif
	let pos = getcurpos()
	let row = pos[1]
	let col = pos[2]
	for [k,v] in items(b:func_names)
	  if v.beg.line <= row && v.end.line >= row
	    if a:gotoBeg
	      call cursor(v.beg.line, v.beg.char)
	    else
	      call cursor(v.end.line, v.end.char)
	    endif
	  endif
	endfor
endfunction

map <silent> <Home> :call GoToFunc(1)<CR>
map <silent> <End> :call GoToFunc(0)<CR>

set statusline=%f
set statusline+=\ -\ [%{StatuslineFunc()}]
