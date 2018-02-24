execute pathogen#infect()
execute pathogen#helptags()

colorscheme apprentice
let mapleader = ","

" Match parens
set showmatch
hi MatchParen cterm=none ctermbg=green ctermfg=blue

" Shared X11 Clipboard
if has('unix')
  if has('mac')       " osx
    set clipboard=unnamed
  else                " linux, bsd, etc
    set clipboard=unnamedplus
  endif
endif

let NERDTreeIgnore=['\.pyc$',  '\.o$', '^__pycache__$', '.*\.js$', '.*\.map$', 'node_modules', '\.git$', '\.svn$']
nnoremap <Leader>n :NERDTreeFind<CR>
set wildignore+=*/tmp/*,*.so,*.swp,*.zip,*/vendor/*,*/\.git/*

" Use silver surfer for grep
if executable('ag')
  set grepprg=ag\ --nogroup\ --nocolor
endif
command! -nargs=+ -complete=file -bar Ag silent! grep! <args>|cwindow|redraw!

" bind K to grep word under cursor
nnoremap K :grep! "\b<C-R><C-W>\b"<CR>:cw<CR>
" vnoremap K :grep! "<C-R>"" %<CR>

" bind \ (backward slash) to grep shortcut
nnoremap \ :Ag<SPACE>

" FZF
set rtp+=~/.fzf
command! MakeTags silent !ctags -R --exclude=/git --exclude=node_modules .
nnoremap <C-p> :Files<CR>
nnoremap <C-t> :Tags<CR>
nnoremap <leader><tab> :BLines<CR>
nnoremap <leader>q :BTags<CR>

command! -bang -nargs=* Ag
  \ call fzf#vim#ag(<q-args>,
  \                 <bang>0 ? fzf#vim#with_preview('up:60%')
  \                         : fzf#vim#with_preview('right:50%:hidden', '?'),
  \                 <bang>0)

" Likewise, Files command with preview window
command! -bang -nargs=? -complete=dir Files
  \ call fzf#vim#files(<q-args>, fzf#vim#with_preview(), <bang>0)

" nnoremap <leader><tab> <plug>(fzf-maps-n)
let g:fzf_history_dir = '~/.local/share/fzf-history'

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
nnoremap <leader>s :mksession<CR>

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
nnoremap <F5> <esc>:SignifyDisable<CR>:set nonumber<CR>:IndentLinesToggle<CR>
nnoremap <F6> <esc>:SignifyDisable<CR>:set number<CR>:IndentLinesToggle<CR>

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
let g:airline_theme='molokai'
let g:airline_left_sep=''
let g:airline_right_sep=''

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
let g:python_highlight_all = 1

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
let g:ale_echo_msg_format = '[%linter%] %s [%severity%]'
let g:ale_python_pycodestyle_options = '--max-line-length 90'
let g:ale_linters = {
\   'python': ['flake8', 'pycodestyle']
\}
let g:ale_fixers = {
\   'javascript': ['eslint'],
\}
let g:ale_python_flake8_executable = '/etc/anaconda/3/bin/python'
let g:ale_python_flake8_args = '-m flake8 --ignore E501'
let g:ale_open_list = 1
let g:ale_keep_list_window_open = 0
let g:ale_lint_on_save = 1
let g:ale_lint_on_text_changed = 0

" YAPF
nnoremap <leader>L :ALEFix<CR>
" autocmd FileType python nnoremap <buffer><Leader>L :<C-u>Yapf<CR>
" let g:yapf#extra_args='--style="{based_on_style: facebook, indent_width: 4}"'
" let g:yapf#code_style='facebook'
autocmd FileType python nnoremap <buffer><leader>L :0,$!yapf --style="{based_on_style: google}"<Cr><C-o>
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
noremap <C-j> <C-W>j
noremap <C-k> <C-W>k
noremap <C-h> <C-W>h
noremap <C-l> <C-W>l

" Moving around in wrapped lines:
autocmd FileType html,markdown,text nnoremap <expr> j v:count ? 'j' : 'gj'
autocmd FileType html,markdown,text nnoremap <expr> k v:count ? 'k' : 'gk'
autocmd FileType html,markdown,text vnoremap <expr> j v:count ? 'j' : 'gj'
autocmd FileType html,markdown,text vnoremap <expr> k v:count ? 'k' : 'gk'

" YouCompleteMe
let g:ycm_python_binary_path = '/etc/anaconda/3/bin/python'
"let g:ycm_python_binary_path = '/usr/bin/python3'
nnoremap <leader>g <ESC>:YcmCompleter GoTo<CR>

" Redraw hacks to force refresh
set ttyfast
set lazyredraw

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
"  " Lightline ALE
"  let g:lightline.component_expand = {
"        \  'linter_warnings': 'lightline#ale#warnings',
"        \  'linter_errors': 'lightline#ale#errors',
"        \  'linter_ok': 'lightline#ale#ok',
"        \ }
"  let g:lightline.component_type = {
"        \     'linter_warnings': 'warning',
"        \     'linter_errors': 'error',
"        \ }
"  let g:lightline.active = { 'right': [[ 'linter_errors', 'linter_warnings', 'linter_ok' ]] }
"  
let g:ycm_enable_autocmd_on_diagnostic_change = 1
" Lightline YCM
"      \  'linter_warnings': 'youcompleteme#GetWarningCount()',
"      \  'linter_errors': 'youcompleteme#GetErrorCount()',

let g:lightline.component_expand = {
      \  'linter_errors': 'ale#statusline#Count(bufnr('')).error'
      \ }
let g:lightline.component_type = {
      \     'linter_errors': 'error',
      \ }
let g:lightline.active = {
      \   'right': [ [ 'lineinfo' ],
      \              [ 'percent' ],
      \              [ 'fileformat', 'fileencoding', 'filetype' ],
      \              [ 'linter_errors' ] ] }
