execute pathogen#infect()
execute pathogen#helptags()

call plug#begin('~/.vim/plugged')
Plug 'neoclide/coc.nvim', {'branch': 'release'}
Plug 'tpope/vim-unimpaired'
Plug 'dense-analysis/ale'
Plug 'itchyny/lightline.vim'
Plug 'jpalardy/vim-slime'
Plug 'tpope/vim-fugitive'
Plug 'Yggdroot/indentLine'
Plug 'junegunn/fzf', { 'do': { -> fzf#install() } }
Plug 'junegunn/fzf.vim'
Plug 'preservim/nerdtree'
Plug 'rdnetto/YCM-Generator', { 'branch': 'stable'}
Plug 'thesis/vim-solidity'
Plug 'SirVer/ultisnips'
Plug 'prabirshrestha/asyncomplete.vim'
Plug 'prabirshrestha/async.vim'
Plug 'prabirshrestha/vim-lsp'
Plug 'prabirshrestha/asyncomplete-lsp.vim'
Plug 'thomasfaingnaert/vim-lsp-snippets'
Plug 'thomasfaingnaert/vim-lsp-ultisnips'
call plug#end()

let g:UltiSnipsExpandTrigger="<TAB>"
let g:UltiSnipsJumpForwardTrigger="<TAB>"
let g:UltiSnipsJumpBackwardTrigger="<S-TAB>"
let g:asyncomplete_auto_completeopt = 0
set completeopt=menuone,noinsert

if executable('hoon-language-server')
    au User lsp_setup call lsp#register_server({
        \ 'name': 'hoon-language-server',
        \ 'cmd': ['hoon-language-server'],
        \ 'whitelist': ['hoon'],
        \ })
endif

colorscheme apprentice
let mapleader = ","

" Redraw hacks to force refresh
set lazyredraw
set ttyfast

" Deferred match parens: https://github.com/andymass/vim-matchup#deferred-highlighting
let g:matchup_matchparen_deferred = 1
hi MatchParen ctermbg=blue guibg=lightblue cterm=italic gui=italic

if &term =~ '256color'
	" Disable Background Color Erase (BCE) so that color schemes
	" work properly when Vim is used inside tmux and GNU screen.
	set t_ut=
endif

" Match parens
set showmatch
hi MatchParen gui=bold guibg=blue guifg=blue
" hi MatchParen cterm=bold ctermbg=none ctermfg=magenta

" Shared X11 Clipboard
if has('unix')
	if has('mac')       " osx
		set clipboard=unnamed
	else                " linux, bsd, etc
		set clipboard=unnamedplus
	endif
endif

let NERDTreeShowHidden=1
let NERDTreeIgnore=['\.pyc$',  '\.o$', '^__pycache__$', '.*\.js$', '.*\.map$', 'node_modules', '\.git$', '\.svn$']
nnoremap <Leader>n :NERDTreeFind<CR>
set wildignore+=*/tmp/*,*.so,*.swp,*.zip,*/vendor/*,*/\.git/*

" Use silver surfer for grep
if executable('ag')
	set grepprg=ag\ --nogroup\ --nocolor
endif
command! -nargs=+ -complete=file -bar Ag silent! grep! <args>|cwindow|redraw!
" command! -nargs=+ -complete=file -bar Ag silent! grep! <args>|redraw!

" bind K to grep word under cursor
nnoremap K :grep! "\b<C-R><C-W>\b"<CR>:cw<CR>
" vnoremap K :grep! "<C-R>"" %<CR>

" bind \ (backward slash) to grep shortcut
nnoremap \ :Ag<SPACE>

" Navigate buffers
nnoremap <bs> <c-^>

" Light cursor column
" set cursorcolumn

" FZF
set rtp+=~/.fzf
set rtp+=/usr/local/opt/fzf
set rtp+=/usr/local/bin/ctags
command! MakeTags silent !ctags -R --exclude=/git --exclude=node_modules .
nnoremap <C-p> :Files<CR>
nnoremap <C-t> :Tags<CR>
nnoremap <leader><tab> :BLines<CR>
nnoremap <leader>q :BTags<CR>
" command! -bang -nargs=* Rg
"   \ call fzf#vim#grep(
"   \   'rg --column --line-number --no-heading --color=always --smart-case '.shellescape(<q-args>), 1,
"   \   <bang>0 ? fzf#vim#with_preview('up:60%')
"   \           : fzf#vim#with_preview('right:50%:hidden', '?'),
"   \   <bang>0)
command! -bang -nargs=* Fd
			\ call fzf#vim#ag(<q-args>,
			\                 <bang>0 ? fzf#vim#with_preview('up:40%')
			\                         : fzf#vim#with_preview('right:50%:hidden', '?'),
			\                 <bang>0)
command! -bang -nargs=? -complete=dir Files
			\ call fzf#vim#files(<q-args>, fzf#vim#with_preview(), <bang>0)
let g:fzf_history_dir = '~/.local/share/fzf-history'
let g:fzf_colors =
			\ { 'fg':      ['fg', 'Normal'],
			\ 'bg':      ['bg', 'Normal'],
			\ 'fg+':     ['fg', 'CursorLine', 'CursorColumn', 'Normal'],
			\ 'bg+':     ['bg', 'CursorLine', 'CursorColumn'],
			\ 'info':    ['fg', 'PreProc'],
			\ 'border':  ['fg', 'Ignore'],
			\ 'prompt':  ['fg', 'Conditional'],
			\ 'pointer': ['fg', 'Exception'],
			\ 'marker':  ['fg', 'Keyword'],
			\ 'spinner': ['fg', 'Label'],
			\ 'header':  ['fg', 'Comment'] }

" PyDocString
let g:pydocstring_templates_dir = '~/.vim/bundle/vim-pydocstring/test/templates/numpy'
nmap <silent> <C-_> <Plug>(pydocstring)

" Buffer back/forward like C-o and C-I
nmap <leader>r :bp<CR>
nmap <leader>f :bn<CR>

" GFM
let g:vim_markdown_preview_browser='Google Chrome'
let g:vim_markdown_preview_github=1

" Undo
if has("persistent_undo")
	set undodir=~/.undodir/
	set undofile
endif

" ,y and ,p for cross-vim clipboard w/o X11
vmap <leader>y :w! /tmp/vitmp<CR>
nmap <leader>p :r! cat /tmp/vitmp<CR>

" Create session, continue w/ vim -S
" nnoremap <leader>s :mksession<CR>
"
" Git blame for current line
nnoremap <Leader>s :<C-u>call gitblame#echo()<CR>

" NERDTree
nnoremap <leader>k :NERDTreeToggle<CR>
let NERDTreeIgnore = ['\.pyc$', '\.ipynb$', '\.png$', '\.jpeg$', '\.jpg$', '\.JPEG$', '\.JPG$', '\.pkl$', '\.ckpt$']

" TagBar
nnoremap <F8> :TagbarToggle<CR>
let g:tagbar_compact = 1
let g:tagbar_show_linenumbers = 1
let g:tagbar_autopreview = 0
let g:tagbar_sort = 0
let g:tagbar_autofocus = 1
let g:tagbar_autoclose = 1
set previewheight=25

" Folding for Python; should move this to ftplugin
nnoremap <space> za
vnoremap <space> zf
nnoremap <silent> <leader>zj :call NextClosedFold('j')<cr>
nnoremap <silent> <leader>zk :call NextClosedFold('k')<cr>
function! NextClosedFold(dir)
    let cmd = 'norm!z' . a:dir
    let view = winsaveview()
    let [l0, l, open] = [0, view.lnum, 1]
    while l != l0 && open
        exe cmd
        let [l0, l] = [l, line('.')]
        let open = foldclosed(l) < 0
    endwhile
    if open
        call winrestview(view)
    endif
endfunction

" YAPF
nnoremap <leader>L :call Yapf("--style='{based_on_style: facebook, coalesce_brackets: true}'")<CR>
" Line Numbers
set nu

"  Yank/Paste clipboard
" nnoremap <C-y> "+y
" vnoremap <C-y> "+y
" nnoremap <C-p> "+gP
" vnoremap <C-p> "+gP
set pastetoggle=<F2>

" Move to normal mode when using arrows
inoremap <silent> <Up> <ESC><Up>
inoremap <silent> <Down> <ESC><Down>

" Make jj ESC
inoremap jj <Esc>

" Toggle gutter shit
nnoremap <F5> <esc>:SignifyDisable<CR>:set nonumber<CR>:IndentLinesToggle<CR>:set nofoldenable<CR>
nnoremap <F6> <esc>:SignifyDisable<CR>:set number<CR>:IndentLinesToggle<CR>:set foldenable<CR>

" Clear highlighting on ESC in normal
" nnoremap <esc> :noh<return>:SyntasticReset<return>:ALEReset<return>:cclose<return><esc>
nnoremap <esc> :noh<return>:ALEReset<return>:cclose<return>:pclose<return><esc>
nnoremap <esc>^[ <esc>^[

" Seems criminal but...
" set mouse=a

" Vim Slimeness
let g:slime_target = "tmux"
let g:slime_python_ipython = 1

" Airline
" let g:airline_theme='molokai'
" let g:airline_left_sep=''
" let g:airline_right_sep=''

" Vim Markdown conceals stuff. Don't.  Also don't fold.
" set nofoldenable
let g:vim_markdown_conceal = 0
let g:vim_markdown_folding_disabled = 1
let g:vim_markdown_fenced_languages = ['bash=sh', 'shell=sh', 'c++=cpp', 'viml=vim', 'ini=dosini']

" Pandoc
" let g:pandoc#syntax#conceal#use = 0

" Enable spell checking for markdown files
au BufRead *.md setlocal spell
au BufRead *.mdown setlocal spell

" Python highlighting
" let g:python_highlight_all = 1

" Goyo Distraction free editing
let g:goyo_height = '100%'
let g:goyo_width = '90%'
let g:goyo_pads = 0
nnoremap <Leader>z :Goyo<CR>i<Esc>`^

" Documentation
"let g:pymode_doc = 1
let g:pymode_doc_key = 'D'

" Syntax
filetype on
syntax on
set ignorecase
set smartcase

" Search highlight/Incremental
set hlsearch
set incsearch
set modelines=0
set wildmenu
set wildmode=longest:full

filetype plugin indent on

set fillchars=vert:│    " that's a vertical box-drawing character
" UTF-8
set encoding=utf-8

" ALE
let g:ale_echo_msg_error_str = 'E'
let g:ale_echo_msg_warning_str = 'W'
let g:ale_echo_msg_format = '(%code%): %s' " '[%linter%] %s [%severity%]'
let g:ale_python_pycodestyle_options = '--max-line-length 90'
let g:ale_linters = {
	\   'python': ['flake8']
	\}
let g:ale_fixers = {
	\   'javascript': ['eslint'],
	\   'json': ['fixjson'],
	\   'python': ['black'],
	\   'cpp': ['clang-format', 'remove_trailing_lines', 'trim_whitespace']
\}
" let g:ale_python_flake8_executable = 'python'
let g:ale_python_flake8_executable = '/home/parag/anaconda3/bin/flake8'
let g:ale_python_flake8_options = '--ignore=E501,W503,E231,E203,W605 --max-line-length=100'
let g:ale_open_list = 1
" let g:ale_keep_list_window_open = 0
let g:ale_lint_on_save = 1
let g:ale_lint_on_text_changed = 'never'
let g:ale_lint_on_insert_leave = 0
let g:ale_lint_on_enter = 0
nnoremap <leader>l :ALELint<CR>
nnoremap <leader>L :ALEFix<CR>

" YAPF
" autocmd FileType python nnoremap <buffer><Leader>L :<C-u>Yapf<CR>
" let g:yapf#extra_args='--style="{based_on_style: facebook, indent_width: 4}"'
" let g:yapf#code_style='facebook'
" autocmd FileType python nnoremap <buffer><leader>L :0,$!yapf --style="{based_on_style: google}"<Cr><C-o>
autocmd FileType cpp nnoremap <buffer><leader>L :ClangFormat<Cr><C-o>
" if you don't want linters to run on opening a file
" let g:ale_lint_on_enter = 0<Paste>


" Syntastic Error/Warn
" let g:syntastic_python_checkers = ['python', 'flake8']
" set statusline+=%#warningmsg#
" set statusline+=%{SyntasticStatuslineFlag()}
" set statusline+=%*
" let g:syntastic_aggregate_errors = 1
" let g:syntastic_always_populate_loc_list = 1
" let g:syntastic_auto_loc_list = 1
" let g:syntastic_check_on_open = 1
" let g:syntastic_check_on_wq = 0
" let g:syntastic_python_ignore_errors = ['E501']
" nnoremap <leader>r :SyntasticReset<CR>

" Javascript
let g:javascript_plugin_jsdoc = 1
let g:jsx_ext_required = 0

" Markdown Ctag
nnoremap <leader>c <ESC>:!ctags -R --exclude=/git --exclude=node_modules .<CR>
let g:tagbar_type_markdown = {
			\ 'ctagstype' : 'markdown',
			\ 'kinds' : [
			\ 'h:headings',
			\ 'l:links',
			\ 'i:images'
			\ ],
			\ "sort" : 0
			\ }

" UndoTree
nnoremap <leader>u <ESC>:UndotreeToggle<cr>
let g:undotree_HighlightChangedText = 1
let g:undotree_HighlightSyntaxAdd = "DiffAdd"
let g:undotree_HighlightSyntaxChange = "DiffChange"
let g:undotree_WindowLayout = 2

let g:lightline = {
			\ 'colorscheme': 'jellybeans',
			\ }
"" tabs
"let g:airline#extensions#tabline#enabled = 1
"let g:airline#extensions#tabline#left_sep = ' '
"let g:airline#extensions#tabline#left_alt_sep = '|'
"let g:airline#extensions#tabline#right_alt_sep = ''
"
"" configure the minimum number of tabs needed to show the tabline.
"let g:airline#extensions#tabline#tab_min_count = 2
"let g:airline#extensions#tabline#show_buffers = 0
"
"" tab shortcuts
nnoremap <C-b>  :tabprevious<CR>
nnoremap <C-B>  :tabnext<CR>
nnoremap <C-c>  :tabnew<CR>
nnoremap <C-x>  :tabclose<CR>

" Easier Pane Navigation
nnoremap <C-j> <C-W>j
nnoremap <C-k> <C-W>k
nnoremap <C-h> <C-W>h
nnoremap <C-l> <C-W>l

" Moving around in wrapped lines:
autocmd FileType html,markdown,text nnoremap <expr> j v:count ? 'j' : 'gj'
autocmd FileType html,markdown,text nnoremap <expr> k v:count ? 'k' : 'gk'
autocmd FileType html,markdown,text vnoremap <expr> j v:count ? 'j' : 'gj'
autocmd FileType html,markdown,text vnoremap <expr> k v:count ? 'k' : 'gk'

set listchars=tab:\|\
set list

" YouCompleteMe
" let g:ycm_echo_current_diagnostic = 0
" Don't cate about popup on overss
let g:ycm_auto_hover = ''
" We dont care about autocomplete
let g:ycm_auto_trigger = 0
let g:ycm_python_binary_path = '/home/parag/anaconda3/envs/tdnast/bin/python'
" let g:ycm_python_binary_path = '/etc/anaconda/3/bin/python'
" let g:ycm_python_binary_path = '/Users/pkmital/anaconda3/bin/python'
" let g:ycm_python_binary_path = '/Users/pkmital/anaconda3/bin/python'
let g:ycm_min_num_identifier_candidate_chars = 1000
let g:ycm_global_ycm_extra_conf = '~/.vim/bundle/YouCompleteMe/.ycm_extra_conf.py'
" let g:ycm_python_binary_path = '/usr/bin/python3'
" let g:ycm_python_binary_path = '/Users/pkmital/anaconda3/bin/python'
nnoremap <leader>g <ESC>:YcmCompleter GoTo<CR>
nnoremap <leader>d <ESC>:YcmCompleter GetDoc<CR>

" These didn't seem to help w/ my issues w/ markdown + tmux
" au FileWritePost * :redraw!
" au TermResponse * :redraw!
" au TextChanged * :redraw!
" au QuickFixCmdPre * :redraw!
" au QuickFixCmdPost * :redraw!

" Grammarous
nnoremap <leader>z <ESC>:GrammarousCheck<CR>

" Last place editing
if has("autocmd")
	au BufReadPost * if line("'\"") > 0 && line("'\"") <= line("$")
				\| exe "normal! g'\"" | endif
endif

" Retab 2 -> 4
nnoremap <leader>t <ESC>:set ts=2 noet<CR>:retab!<CR>:set et ts=4<CR>:retab<CR>
vnoremap <leader>t <ESC>:set ts=2 noet<CR>:'<,'>retab!<CR>:set et ts=4<CR>:'<,'>retab<CR>

nnoremap <leader>T <ESC>:set ts=4 noet<CR>:retab!<CR>:set et ts=2<CR>:retab<CR>
vnoremap <leader>T <ESC>:set ts=4 noet<CR>:'<,'>retab!<CR>:set et ts=2<CR>:'<,'>retab<CR>

" Turn off automatic visual selection w/ mouse
set mouse-=a

" Delays
set timeoutlen=1000 ttimeoutlen=0

" Tired of lowercasing stuff by mistake
vnoremap u :noh<cr>

"let g:indent_guides_enable_on_vim_startup = 1
"let g:indent_guides_start_level = 2
"let g:indent_guides_guide_size = 1
"hi IndentGuidesOdd  ctermbg=grey
"hi IndentGuidesEven ctermbg=darkgrey
let g:indentLine_char = '┆'
"'▏'
"'︳'
"
" Lightline ALE
let g:lightline.component_expand = {
			\  'linter_warnings': 'lightline#ale#warnings',
			\  'linter_errors': 'lightline#ale#errors',
			\  'linter_ok': 'lightline#ale#ok',
			\ }
let g:lightline.component_type = {
			\     'linter_warnings': 'warning',
			\     'linter_errors': 'error',
			\ }
let g:lightline.active = { 'right': [[ 'linter_checking', 'linter_errors', 'linter_warnings', 'linter_ok' ]] }
" let g:lightline.active = { 'right': [[ 'linter_errors', 'linter_warnings', 'linter_ok' ]] }

let g:ycm_enable_autocmd_on_diagnostic_change = 1
" Lightline YCM
"      \  'linter_warnings': 'youcompleteme#GetWarningCount()',
"      \  'linter_errors': 'youcompleteme#GetErrorCount()',

" let g:lightline.component_expand = {
"       \  'linter_errors': 'ale#statusline#Count(bufnr('')).error'
"       \ }
let g:lightline.active = {
			\   'right': [ [ 'lineinfo' ],
			\              [ 'percent' ],
			\              [ 'fileformat', 'fileencoding', 'filetype' ],
			\              [ 'linter_checking', 'linter_errors', 'linter_warnings', 'linter_ok' ] ] }

" Conque
" let g:ConqueTerm_Color = 2         " 1: strip color after 200 lines, 2: always with color
" let g:ConqueTerm_CloseOnEnd = 1    " close conque when program ends running
" let g:ConqueTerm_StartMessages = 0 " display warning messages if conqueTerm is configured incorrectly  
"
" vim-multiple-cursors Setup {{{
function! Multiple_cursors_before()
	call youcompleteme#DisableCursorMovedAutocommands()
endfunction

function! Multiple_cursors_after()
	call youcompleteme#EnableCursorMovedAutocommands()
endfunction
" }}}
"
" Highlight all instances of word under cursor, when idle.
" Useful when studying strange source code.
" Type z/ to toggle highlighting on/off.
nnoremap z/ :if AutoHighlightToggle()<Bar>set hls<Bar>endif<CR>
function! AutoHighlightToggle()
   let @/ = ''
   if exists('#auto_highlight')
     au! auto_highlight
     augroup! auto_highlight
     setl updatetime=4000
     echo 'Highlight current word: off'
     return 0
  else
    augroup auto_highlight
    au!
    au CursorHold * let @/ = '\V\<'.escape(expand('<cword>'), '\').'\>'
    augroup end
    setl updatetime=500
    echo 'Highlight current word: ON'
  return 1
 endif
endfunction

" autocmd User CocOpenFloat call nvim_win_set_config(g:coc_last_float_win, {'relative': 'editor', 'row': 0, 'col': 0})
" autocmd User CocOpenFloat call nvim_win_set_width(g:coc_last_float_win, 9999)
